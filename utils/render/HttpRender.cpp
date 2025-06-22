#include "../../settings.hpp"
#include "HttpRender.hpp"

// read html file
std::string readHtmlFile(const std::string& filepath) {
    std::string compelete_file_path = TEMPLATES_DIR + "/" + filepath;
    // std::cout << compelete_file_path << std::endl;
    std::ifstream file(compelete_file_path);
    if (!file.is_open()) {
        return "<html><body><h1>Error: File not found</h1></body></html>";
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// Helper function to get string representation of any ContextValue
std::string getStringValue(const ContextValue& val) {
    if (std::holds_alternative<ContextString>(val.value)) {
        return std::get<ContextString>(val.value);
    } else if (std::holds_alternative<ContextObject>(val.value)) {
        return "[object]";  // Could be customized
    } else {
        const auto& arr = std::get<ContextArray>(val.value);
        return "[array of " + std::to_string(arr.size()) + " items]";
    }
}

// Helper function to check if a value is "truthy"
bool isTruthy(const ContextValue& val) {
    if (std::holds_alternative<ContextString>(val.value)) {
        const std::string& str = std::get<ContextString>(val.value);
        return !str.empty() && str != "false" && str != "0";
    } else if (std::holds_alternative<ContextArray>(val.value)) {
        const auto& arr = std::get<ContextArray>(val.value);
        return !arr.empty();
    } else {
        const auto& obj = std::get<ContextObject>(val.value);
        return !obj.empty();
    }
}

// Helper function to access nested properties (e.g., "user.username")
ContextValue getNestedValue(const Context& context, const std::string& key) {
    size_t dotPos = key.find('.');
    if (dotPos == std::string::npos) {
        // Simple key lookup
        auto it = context.find(key);
        if (it != context.end()) {
            return it->second;
        }
        return ContextValue("");
    }
    
    // Nested key lookup
    std::string rootKey = key.substr(0, dotPos);
    std::string remainingKey = key.substr(dotPos + 1);
    
    auto it = context.find(rootKey);
    if (it != context.end() && std::holds_alternative<ContextObject>(it->second.value)) {
        const auto& obj = std::get<ContextObject>(it->second.value);
        return getNestedValue(obj, remainingKey);
    }
    
    return ContextValue("");
}

std::string parseHtmlCode(const std::string &html, Context& context) {
    std::string rendered;
    size_t pos = 0;

    while (pos < html.length()) {
        size_t openTag = html.find("{{", pos);

        // No more tags
        if (openTag == std::string::npos) {
            rendered += html.substr(pos);
            break;
        }

        rendered += html.substr(pos, openTag - pos);
        size_t closeTag = html.find("}}", openTag);
        if (closeTag == std::string::npos) break;

        std::string tagContent = html.substr(openTag + 2, closeTag - openTag - 2);
        tagContent.erase(0, tagContent.find_first_not_of(" \t"));
        tagContent.erase(tagContent.find_last_not_of(" \t") + 1);

        // ---- FOR LOOP ----
        if (tagContent.rfind("for ", 0) == 0) {
            // Parse "for variable in collection"
            std::string forExpression = tagContent.substr(4); // Remove "for "
            forExpression.erase(0, forExpression.find_first_not_of(" \t"));
            
            size_t inPos = forExpression.find(" in ");
            if (inPos == std::string::npos) {
                std::cerr << "Invalid for loop syntax. Expected: for variable in collection" << std::endl;
                pos = closeTag + 2;
                continue;
            }
            
            std::string variable = forExpression.substr(0, inPos);
            std::string collection = forExpression.substr(inPos + 4);
            variable.erase(0, variable.find_first_not_of(" \t"));
            variable.erase(variable.find_last_not_of(" \t") + 1);
            collection.erase(0, collection.find_first_not_of(" \t"));
            collection.erase(collection.find_last_not_of(" \t") + 1);

            size_t forStart = closeTag + 2;
            size_t endForTag = html.find("{{ endfor }}", forStart);
            
            if (endForTag == std::string::npos) {
                std::cerr << "Missing {{ endfor }} for for block." << std::endl;
                break;
            }

            std::string loopContent = html.substr(forStart, endForTag - forStart);
            
            // Get the collection value
            ContextValue collectionValue = getNestedValue(context, collection);
            
            if (std::holds_alternative<ContextArray>(collectionValue.value)) {
                const auto& items = std::get<ContextArray>(collectionValue.value);
                
                for (const ContextValue& item : items) {
                    // Create a temporary context with the loop variable
                    Context tempContext = context;
                    tempContext[variable] = item;
                    rendered += parseHtmlCode(loopContent, tempContext);
                }
            }

            pos = endForTag + 12; // Skip past {{ endfor }}
        }

        // ---- IF STATEMENT ----
        else if (tagContent.rfind("if ", 0) == 0) {
            std::string conditionKey = tagContent.substr(3);
            conditionKey.erase(0, conditionKey.find_first_not_of(" \t"));
            conditionKey.erase(conditionKey.find_last_not_of(" \t") + 1);

            ContextValue conditionValue = getNestedValue(context, conditionKey);
            bool conditionTrue = isTruthy(conditionValue);

            size_t ifStart = closeTag + 2;
            size_t elseTag = html.find("{{ else }}", ifStart);
            size_t endTag = html.find("{{ end }}", ifStart);

            if (endTag == std::string::npos) {
                std::cerr << "Missing {{ end }} for if block." << std::endl;
                break;
            }

            if (conditionTrue) {
                if (elseTag != std::string::npos && elseTag < endTag) {
                    rendered += parseHtmlCode(html.substr(ifStart, elseTag - ifStart), context);
                } else {
                    rendered += parseHtmlCode(html.substr(ifStart, endTag - ifStart), context);
                }
            } else {
                if (elseTag != std::string::npos && elseTag < endTag) {
                    rendered += parseHtmlCode(html.substr(elseTag + 10, endTag - (elseTag + 10)), context);
                }
            }

            pos = endTag + 9;  // Skip past {{ end }}
        }

        // ---- VARIABLE REPLACEMENT ----
        else {
            std::string key = tagContent;
            ContextValue value = getNestedValue(context, key);
            rendered += getStringValue(value);
            pos = closeTag + 2;
        }
    }

    return rendered;
}


// Has to take context after
void renderHtml(Request request ,const std::string& filepath, Context& context){
    std::string html = readHtmlFile(filepath);
    std::string rendered = parseHtmlCode(html, context);
    
    std::string cookie = "session_id=" + request.get_session_id() + "; Path=/; HttpOnly";

    std::string http_response =
                "HTTP/1.1 200 OK\r\n" 
                "Content-Type: text/html\r\n" 
                "Set-Cookie: " + cookie + "\r\n"
                "Connection: close\r\n" 
                "\r\n" + rendered;

    send(request.getSocket(), http_response.c_str(), http_response.length(), 0);
    close(request.getSocket());
}

// Render text
void renderText(Request request ,const std::string& text){
    std::string http_response =
                "HTTP/1.1 200 OK\r\n" 
                "Content-Type: text/html\r\n" 
                "Connection: close\r\n" 
                "\r\n" + text;

    send(request.getSocket(), http_response.c_str(), http_response.length(), 0);
    close(request.getSocket());
}

// Redirect
void redirect(const Request& request, const std::string& targetUrl, int statusCode) {
    std::string statusText;

    if (statusCode == 301)
        statusText = "301 Moved Permanently";
    else
        statusText = "302 Found";  // Default

    std::string response =
        "HTTP/1.1 " + statusText + "\r\n"
        "Location: " + targetUrl + "\r\n"
        "Content-Length: 0\r\n"
        "Connection: close\r\n\r\n";

    send(request.getSocket(), response.c_str(), response.length(), 0);
    close(request.getSocket());
}
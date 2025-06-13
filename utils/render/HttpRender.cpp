#include<map>
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

// Helper functon parse the html code
std::string parseHtmlCode(const std::string &html, std::map<std::string, std::string>& context) {
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

        // ---- IF STATEMENT ----
        if (tagContent.rfind("if ", 0) == 0) {
            std::string conditionKey = tagContent.substr(3);
            conditionKey.erase(0, conditionKey.find_first_not_of(" \t"));
            conditionKey.erase(conditionKey.find_last_not_of(" \t") + 1);

            bool conditionTrue = context.find(conditionKey) != context.end() && context[conditionKey] != "false" && context[conditionKey] != "0";

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
                    rendered += parseHtmlCode(html.substr(elseTag + 9, endTag - (elseTag + 9)), context);
                }
            }

            pos = endTag + 9;  // Skip past {{ end }}
        }

        // ---- VARIABLE REPLACEMENT ----
        else {
            std::string key = tagContent;
            std::string value = (context.find(key) != context.end()) ? context[key] : "";
            rendered += value;
            pos = closeTag + 2;
        }
    }

    return rendered;
}



// Has to take context after
void renderHtml(Request request ,const std::string& filepath, std::map<std::string, std::string>& context){
    std::string html = readHtmlFile(filepath);
    std::string rendered = parseHtmlCode(html, context);

    std::string http_response =
                "HTTP/1.1 200 OK\r\n" 
                "Content-Type: text/html\r\n" 
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
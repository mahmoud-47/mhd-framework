#include "Request.hpp"
#include <algorithm> // For std::transform

/**********
 *   Computations 
 */

static std::string urlDecode(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); ++i) {
        if (str[i] == '+') {
            result += ' ';
        } else if (str[i] == '%' && i + 2 < str.length()) {
            std::string hex = str.substr(i + 1, 2);
            result += static_cast<char>(std::stoi(hex, nullptr, 16));
            i += 2;
        } else {
            result += str[i];
        }
    }
    return result;
}

/**
 * Set the url from the buffer
 */
void Request::setUrlFromBuffer(){
    std::istringstream stream(buffer);
    stream >> method;  // method is the first token (e.g., "GET")
}
/**
 * Set the method from the buffer
 */
void Request::setMethodFromBuffer(){
    std::istringstream stream(buffer);
    std::string temp;
    stream >> temp;    // Skip method
    stream >> url;     // Get URL 
}
/**
 * Set user-agent from the buffer
 */
void Request::setUserAgentFromBuffer(){
    std::istringstream stream(buffer);
    std::string line;

    while (std::getline(stream, line)) {
        if (line.find("User-Agent:") == 0) {
            userAgent = line.substr(12); // Skip "User-Agent: "
            // Trim any leading whitespace if needed
            size_t start = userAgent.find_first_not_of(" \t");
            if (start != std::string::npos) {
                userAgent = userAgent.substr(start);
            }
            break;
        }
    }
}
/**
 * Set the Hostname from the buffer
 */
void Request::setHostanameFromBuffer(){
    std::istringstream stream(buffer);
    std::string line;
    while (std::getline(stream, line)) {
        if (line.find("Host:") == 0) {
            size_t start = line.find(' ') + 1;
            size_t end = line.find(':', start);
            if (end != std::string::npos)
                hostname = line.substr(start, end - start);
            else
                hostname = line.substr(start);
            break;
        }
    }
}


/**
 * Pass the buffer to the class
 */
Request::Request(const std::string buffer){
    this->buffer = buffer;
    
    setUrlFromBuffer();
    setMethodFromBuffer();
    setUserAgentFromBuffer();
    setHostanameFromBuffer();
}
/**
 * Get the url in runtime, in string format ex : /mypath/1
 */
std::string Request::getUrl() const{
    return urlDecode(url);
}
/**
 * Get the Method as string exemple : GET / POST / DELETE / PUT
 */
std::string Request::getMethod() const{
    return method;
}
/**
 * Get IP Adress the request comes from
 */
std::string Request::getUserAgent() const{
    return userAgent;
}
/**
 * Get Hostname
 */
std::string Request::getHostName() const{
    return hostname;
}
/**
 * Get Socket
 */
int Request::getSocket() const{
    return socket;
}
/**
 * Set the url pattern
 */
void Request::setUrlFormat(std::string urlFormat){
    this->urlFormat = urlFormat;
}
/**
 * Get Header value
 */
std::string Request::getHeaderValue(const std::string& headerName) {
    std::istringstream stream(buffer);
    std::string line;
    std::string searchKey = headerName + ": ";

    while (std::getline(stream, line)) {
        if (line.find(searchKey) == 0) {  // Line starts with the headerName
            return line.substr(searchKey.length());
        }
        if (line == "\r" || line.empty()) {
            break; // Reached the end of the headers
        }
    }

    return "";
}
/**
 * Set Socket
 */
void Request::setSocket(int socket){
    this->socket = socket;
}
/**
 * Client wants HTML
 */
bool Request::clientWantsHtml() const{
    std::string lowercaseRequest = buffer;
    std::transform(lowercaseRequest.begin(), lowercaseRequest.end(), lowercaseRequest.begin(), ::tolower);

    size_t pos = lowercaseRequest.find("accept:");
    if (pos == std::string::npos) return false;

    size_t end = lowercaseRequest.find("\r\n", pos);
    std::string acceptLine = lowercaseRequest.substr(pos, end - pos);

    return acceptLine.find("text/html") != std::string::npos;
}
/**
 * Client wants Anything
 */
bool Request::clientWantAnything() const{
    std::string lowercaseRequest = buffer;
    std::transform(lowercaseRequest.begin(), lowercaseRequest.end(), lowercaseRequest.begin(), ::tolower);

    size_t pos = lowercaseRequest.find("accept:");
    if (pos == std::string::npos) return false;

    size_t end = lowercaseRequest.find("\r\n", pos);
    std::string acceptLine = lowercaseRequest.substr(pos, end - pos);

    return acceptLine.find("*/*") != std::string::npos;
}
/**
 * Get parameter by parameter name as string - GET method
 */
std::string Request::getGETParameterByParameterName(const std::string parameterName){
    // Remove query parameters
    std::string cleanedUrl = url;
    size_t queryPos = url.find('?');
    if (queryPos != std::string::npos) {
        cleanedUrl = url.substr(0, queryPos);
    }

    std::vector<std::string> pattern_parts, url_parts;
    std::stringstream ss_pattern(urlFormat), ss_url(cleanedUrl);
    std::string token;

    // Split pattern and url by '/'
    while (std::getline(ss_pattern, token, '/')) {
        if (!token.empty()) pattern_parts.push_back(token);
    }
    while (std::getline(ss_url, token, '/')) {
        if (!token.empty()) url_parts.push_back(token);
    }

    if (pattern_parts.size() != url_parts.size())
        return "";

    // Map parameter names to values
    std::unordered_map<std::string, std::string> params;
    for (size_t i = 0; i < pattern_parts.size(); ++i) {
        const std::string& part = pattern_parts[i];
        if (part.size() >= 3 && part.front() == '{' && part.back() == '}') {
            std::string param = part.substr(1, part.size() - 2);
            params[param] = url_parts[i];
        }
    }

    // Return value or empty string
    auto it = params.find(parameterName);
    return (it != params.end()) ? it->second : "";
}

/**
 * Get Query parameter by name (?var=val), returns "" if not found
 */
std::string Request::getQueryParameterByParameterName(const std::string parameterName){
    size_t posQuestionMark = url.find("?");
    if (posQuestionMark != std::string::npos) {
        std::string sub_url = url.substr(posQuestionMark+1);
        std::stringstream ssub_url(sub_url);
        std::vector<std::string> key_vals;
        std::string token;
        while(std::getline(ssub_url, token, '&')){
            if(!token.empty()){
                size_t posEqual = token.find("=");
                if(posEqual != std::string::npos){
                    std::string key = token.substr(0, posEqual);
                    std::string value = token.substr(posEqual + 1);
                    if(key == parameterName)
                        return value;
                }
            }
        }
        
    }
    return "";
}

/*---------------- POST METHOD*/

static std::unordered_map<std::string, std::string> parseUrlEncodedBody(const std::string& body) {
    
    std::unordered_map<std::string, std::string> result;
    
    // Remove any trailing whitespace or carriage returns
    std::string cleanBody = body;
    while (!cleanBody.empty() && (cleanBody.back() == '\r' || cleanBody.back() == '\n' || cleanBody.back() == ' ')) {
        cleanBody.pop_back();
    }
    
    
    // Split by '&' manually instead of using getline
    size_t start = 0;
    size_t pos = 0;
    int pairCount = 0;
    
    while (pos <= cleanBody.length()) {
        // Find next '&' or end of string
        pos = cleanBody.find('&', start);
        if (pos == std::string::npos) {
            pos = cleanBody.length();
        }
        
        
        // Extract the key-value pair
        if (pos > start) {
            std::string pair = cleanBody.substr(start, pos - start);
            
            size_t equalPos = pair.find('=');
            if (equalPos != std::string::npos) {
                std::string key = pair.substr(0, equalPos);
                std::string value = pair.substr(equalPos + 1);
                
                key = urlDecode(key);
                value = urlDecode(value);
                
                
                result[key] = value;
                pairCount++;
            } else {
                //
            }
        }
        
        start = pos + 1;
        if (start > cleanBody.length()) break;
    }
    
    return result;
}


std::string removeWebKitBoundaryLines(const std::string& input) {
    const std::string boundary = "------WebKitFormBoundary";
    std::string result = input;
    
    size_t boundary_pos = result.find(boundary);
    if (boundary_pos != std::string::npos) {
        size_t line_end = result.find_first_of("\r\n", boundary_pos);
        if (line_end != std::string::npos) {
            result.erase(boundary_pos, line_end - boundary_pos);
        } else {
            result.erase(boundary_pos);
        }
        
        while (!result.empty() && (result.back() == '\n' || result.back() == '\r')) {
            result.pop_back();
        }
    }
    
    return result;
}


/**
 * Get Form data parameter by name 
 */
std::string Request::getFormDataParameterByParameterName(const std::string parameterName) {
    std::string contentType = getHeaderValue("Content-Type");
    
    if (contentType.find("multipart/form-data") != std::string::npos) {
        if (!multipartParsed) {
            parseMultipartData();
            multipartParsed = true;
        }
        
        auto it = multipartFields.find(parameterName);
        return it != multipartFields.end() ? removeWebKitBoundaryLines(it->second.value) : "";
    }
    
    size_t headerEnd = buffer.find("\r\n\r\n");
    if (headerEnd == std::string::npos) return "";
    
    std::string body = buffer.substr(headerEnd + 4);
    std::unordered_map<std::string, std::string> params = parseUrlEncodedBody(body);

    auto it = params.find(parameterName);
    return it != params.end() ? it->second : "";
}

void Request::parseMultipartData() {
    std::string contentType = getHeaderValue("Content-Type");
    size_t boundaryPos = contentType.find("boundary=");
    if (boundaryPos == std::string::npos) return;
    
    std::string boundary = "--" + contentType.substr(boundaryPos + 9);
    size_t headerEnd = buffer.find("\r\n\r\n");
    if (headerEnd == std::string::npos) return;
    
    std::string body = buffer.substr(headerEnd + 4);
    size_t finalBoundaryPos = body.find(boundary + "--");
    if (finalBoundaryPos == std::string::npos) finalBoundaryPos = body.length();

    std::vector<std::string> parts;
    size_t pos = 0;
    
    while (pos < finalBoundaryPos) {
        size_t nextBoundary = body.find(boundary, pos);
        if (nextBoundary == std::string::npos || nextBoundary >= finalBoundaryPos) break;
        
        if (pos > 0) {
            std::string part = body.substr(pos, nextBoundary - pos);
            if (part.size() >= 2 && part.substr(0, 2) == "\r\n") part = part.substr(2);
            if (!part.empty()) parts.push_back(part);
        }
        
        pos = nextBoundary + boundary.length();
        if (body.substr(pos, 2) == "\r\n") pos += 2;
    }
    
    if (pos < finalBoundaryPos) {
        std::string finalPart = body.substr(pos, finalBoundaryPos - pos);
        if (finalPart.size() >= 2 && finalPart.substr(0, 2) == "\r\n") finalPart = finalPart.substr(2);
        if (!finalPart.empty()) parts.push_back(finalPart);
    }

    for (const auto& part : parts) {
        parseMultipartPart(part);
    }
}

void Request::parseMultipartPart(const std::string& part) {
    size_t headerEnd = part.find("\r\n\r\n");
    if (headerEnd == std::string::npos) return;
    
    std::string headers = part.substr(0, headerEnd);
    std::string content = part.substr(headerEnd + 4);
    
    size_t contentEnd = content.find_last_not_of("\r\n");
    if (contentEnd != std::string::npos) {
        content = content.substr(0, contentEnd + 1);
    } else {
        content.clear();
    }

    FormField field;
    std::string fieldName;
    
    std::istringstream headerStream(headers);
    std::string line;
    
    while (std::getline(headerStream, line)) {
        if (!line.empty() && line.back() == '\r') line.pop_back();
        
        if (line.find("Content-Disposition:") == 0) {
            size_t namePos = line.find("name=\"");
            if (namePos != std::string::npos) {
                namePos += 6;
                size_t nameEnd = line.find("\"", namePos);
                if (nameEnd != std::string::npos) {
                    fieldName = line.substr(namePos, nameEnd - namePos);
                }
            }
            
            size_t filenamePos = line.find("filename=\"");
            if (filenamePos != std::string::npos) {
                filenamePos += 10;
                size_t filenameEnd = line.find("\"", filenamePos);
                if (filenameEnd != std::string::npos) {
                    field.filename = line.substr(filenamePos, filenameEnd - filenamePos);
                }
            }
        }
        else if (line.find("Content-Type:") == 0) {
            field.contentType = line.substr(13);
            field.contentType.erase(0, field.contentType.find_first_not_of(" \t"));
            field.contentType.erase(field.contentType.find_last_not_of(" \t") + 1);
        }
    }
    
    field.value = content;
    
    if (!fieldName.empty()) {
        multipartFields[fieldName] = field;
    }
}


bool Request::isFileUpload(const std::string& parameterName) {
    std::string contentType = getHeaderValue("Content-Type");

    if (contentType.find("multipart/form-data") != std::string::npos) {
        if (!multipartParsed) {
            parseMultipartData();
            multipartParsed = true;
        }
        
        auto it = multipartFields.find(parameterName);
        return it != multipartFields.end() && !it->second.filename.empty();
    }

    return false;
}

std::string Request::getFileName(const std::string& parameterName) {
    std::string contentType = getHeaderValue("Content-Type");
    
    if (contentType.find("multipart/form-data") != std::string::npos) {
        if (!multipartParsed) {
            parseMultipartData();
            multipartParsed = true;
        }
        
        auto it = multipartFields.find(parameterName);
        return it != multipartFields.end() ? it->second.filename : "";
    }
    
    return "";
}

std::string Request::getFileContentType(const std::string& parameterName) {
    std::string contentType = getHeaderValue("Content-Type");
    
    if (contentType.find("multipart/form-data") != std::string::npos) {
        if (!multipartParsed) {
            parseMultipartData();
            multipartParsed = true;
        }
        
        auto it = multipartFields.find(parameterName);
        return it != multipartFields.end() ? it->second.contentType : "";
    }
    
    return "";
}

MhdFile* Request::getFileFromPostByName(const std::string parameterName){
    // check if file uploaded
    if(! isFileUpload(parameterName))
        return nullptr;

    // get the parameters
    std::string file_content = this->getFormDataParameterByParameterName(parameterName);
    std::string file_name = this->getFileName(parameterName);
    std::string file_content_type = this->getFileContentType(parameterName);

    MhdFile *file = new MhdFile(file_name, file_content, file_content_type);
    return file;
}


// get cookie by name
std::string Request::get_cookie(const std::string &name) const{
    const std::string needle = "\r\nCookie: ";
    size_t start = buffer.find(needle);

    if (start == std::string::npos) {
        // Also check if it's at the beginning (no \r\n before)
        if (buffer.rfind("Cookie: ", 0) == 0) {
            start = 0;
        } else {
            return "";
        }
    } else {
        start += 2; // skip \r\n
    }

    // Extract cookie line
    size_t cookie_line_start = buffer.find("Cookie: ", start);
    size_t end = buffer.find("\r\n", cookie_line_start);
    if (cookie_line_start == std::string::npos || end == std::string::npos)
        return "";

    std::string cookie_line = buffer.substr(cookie_line_start + 8, end - (cookie_line_start + 8));

    // Parse cookies
    std::istringstream stream(cookie_line);
    std::string token;
    while (std::getline(stream, token, ';')) {
        size_t eq = token.find('=');
        if (eq != std::string::npos) {
            std::string key = token.substr(0, eq);
            std::string val = token.substr(eq + 1);

            // Trim whitespace
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            val.erase(0, val.find_first_not_of(" \t"));
            val.erase(val.find_last_not_of(" \t") + 1);

            if (key == name) {
                return val;
            }
        }
    }

    return "";
}
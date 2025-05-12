#include "Request.hpp"
#include <algorithm> // For std::transform

/**********
 *   Computations 
 */

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
    return url;
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
 * Get Query parameter by name (?var=val)
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
/**
 * Get Form data parameter by name 
 */
std::string Request::getFormDataParameterByParameterName(const std::string parameterName) {
    std::string contentType = getHeaderValue("Content-Type");
    std::string boundaryPrefix = "boundary=";
    size_t boundaryPos = contentType.find(boundaryPrefix);

    if (boundaryPos == std::string::npos) {
        return "";
    }

    std::string boundary = "--" + contentType.substr(boundaryPos + boundaryPrefix.length());

    // Separate headers from body
    size_t headerEnd = buffer.find("\r\n\r\n");
    if (headerEnd == std::string::npos) {
        return "";
    }

    std::string body = buffer.substr(headerEnd + 4);
    std::istringstream bodyStream(body);
    std::string line;
    bool foundField = false;
    std::string value;

    while (std::getline(bodyStream, line)) {
        if (line.find(boundary) != std::string::npos) {
            foundField = false; // Reset for each part
        }

        if (line.find("Content-Disposition: form-data;") != std::string::npos &&
            line.find("name=\"" + parameterName + "\"") != std::string::npos) {
            foundField = true;
        } else if (foundField && line == "\r") {
            // Next line contains the value
            std::getline(bodyStream, value);
            // Remove possible trailing \r
            if (!value.empty() && value.back() == '\r') {
                value.pop_back();
            }
            return value;
        }
    }

    return "";
}
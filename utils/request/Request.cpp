#include "Request.hpp"

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
 * Get parameter by parameter name as string
 */
std::string Request::getGETParameterByParameterName(const std::string parameterName){
    
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
 * Set the url pattern
 */
void Request::setUrlFormat(std::string urlFormat){
    this->urlFormat = urlFormat;
}
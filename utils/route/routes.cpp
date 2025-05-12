#include <iostream>
#include "routes.hpp"

// Helper Function to convert a URL pattern with placeholders into a regex pattern
std::string convertPatternToRegex(const std::string& pattern) {
    std::string regexPattern = pattern;
    // Replace {id} with (\w+) to match alphanumeric strings
    regexPattern = std::regex_replace(regexPattern, std::regex("\\{\\w+\\}"), "(\\w+)");
    return regexPattern;
}


/**
 * Add a route
 */
void Route::addRoute(URLPattern route){
    bool found = false;
    for(URLPattern urlpattern : urls){
        if(urlpattern.url == route.url || urlpattern.name == route.name){
            std::cerr << "*** Cannot regster two routes with the same url or name" << std::endl;
            found = true;
            break;
        }
    }
    urls.push_back(route);
}
/**
 * Check if a utl exists
 */
bool Route::hasUrl(const std::string url){
    for(URLPattern urlpattern : urls){
        std::string regexPattern = convertPatternToRegex(urlpattern.url);
        std::regex regex(regexPattern);
        if (std::regex_match(url, regex)) {
            return true;
        }
    }
    return false;
}
/**
 * Get Url pattern from the runtime url, returns a pointer to the URL pattern
 */
URLPattern* Route::getUrlPattern(const std::string url){
    // Remove query parameters
    std::string cleanedUrl = url;
    size_t queryPos = url.find('?');
    if (queryPos != std::string::npos) {
        cleanedUrl = url.substr(0, queryPos);
    }

    for (URLPattern urlpattern : urls) {
        std::string regexPattern = convertPatternToRegex(urlpattern.url);
        std::regex regex(regexPattern);
        if (std::regex_match(cleanedUrl, regex)) {
            URLPattern* result = new URLPattern;
            *result = urlpattern;
            return result;
        }
    }
    return nullptr;
}
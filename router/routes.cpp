#include <iostream>
#include "routes.hpp"

/**
 * Add a route
 */
void Route::addRoute(URLPattern route){
    for(URLPattern urlpattern : urls){
        if(urlpattern.url == route.url || urlpattern.name == route.name){
            std::cerr << "Cannot regster rwo routes with the same url or name" << endl;
        }
    }
    urls.push_back(route);
}
/**
 * Check if a utl exists
 */
bool Route::hasUrl(const char* url){}
/**
 * Get Url pattern from the runtime url, returns a pointer to the URL pattern
 */
URLPattern* Route::getUrlPattern(std::string url){}
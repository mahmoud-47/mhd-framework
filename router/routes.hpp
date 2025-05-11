#ifndef __MHD_ROUTES__
#define __MHD_ROUTES__

#include<vector>
#include<string>
#include "utils/request/Request.hpp"
#include "utils/URLPattern.hpp"


class Route{
    private:
        /**
         * Add your routes, by appending them to the routes vector
         * Example : routes.push_back()
         */
        std::vector<URLPattern> urls;
    public:
        Route() = default;
        /**
         * Add a route
         */
        void addRoute(URLPattern route);
        /**
         * Check if a utl exists
         */
        bool hasUrl(const char* url);
        /**
         * Get Url pattern from the runtime url, returns a pointer to the URL pattern
         */
        static URLPattern* getUrlPattern(std::string url);
};

#endif
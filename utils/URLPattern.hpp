#ifndef __MHD_FRAM_URL_PATTERN__
#define __MHD_FRAM_URL_PATTERN__
#include <string>
#include "request/Request.hpp"
/**
 * URL Pattern
 */
struct URLPattern{
    /**
     * Unique Name of the route
     */
    std::string name;
    /**
     * Unique URL example. /my/path/{id}
     */
    std::string url;
    /**
     * Function from the controllers
     */
    void (*controller)(Request);
};

#endif
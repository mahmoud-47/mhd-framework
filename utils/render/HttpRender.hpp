#ifndef __MHD_HTTP_RENDER__
#define __MHD_HTTP_RENDER__

#include<map>
#include <iostream>
#include <map>
#include <vector>
#include <variant>
#include <sstream>
#include <string>
#include <fstream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utils/request/Request.hpp"
#include "../../settings.hpp"

    // Forward declaration for recursive variant
    struct ContextValue;

    // Define the context value types
    using ContextObject = std::map<std::string, ContextValue>;
    using ContextArray = std::vector<ContextValue>;
    using ContextString = std::string;

    // Recursive variant that can hold strings, objects, or arrays
    struct ContextValue {
        std::variant<ContextString, ContextObject, ContextArray> value;
        
        // Constructors for easy assignment
        ContextValue(const std::string& str) : value(str) {}
        ContextValue(const char* str) : value(std::string(str)) {}
        ContextValue(const ContextObject& obj) : value(obj) {}
        ContextValue(const ContextArray& arr) : value(arr) {}
        ContextValue() : value(std::string("")) {}
    };

    using Context = ContextObject;

    std::string readHtmlFile(const std::string& filepath);

    // Has to take context after
    void renderHtml(Request request ,const std::string& filepath, Context& ontext);

    // Render text
    void renderText(Request request ,const std::string& text);

#endif
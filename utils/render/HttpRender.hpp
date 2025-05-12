#ifndef __MHD_HTTP_RENDER__
#define __MHD_HTTP_RENDER__

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "utils/request/Request.hpp"
#include "../../settings.hpp"

    std::string readHtmlFile(const std::string& filepath);

    // Has to take context after
    void renderHtml(Request request ,const std::string& filepath);

    // Render text
    void renderText(Request request ,const std::string& text);

#endif
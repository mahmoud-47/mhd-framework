#ifndef __MHD_Functions_
#define __MHD_Functions_

#include <string>

bool endsWith(const std::string& str, const std::string& suffix) {
    return str.size() >= suffix.size() && 
           str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

std::string getMimeType(const std::string& path) {
    if (endsWith(path, ".html")) return "text/html";
    if (endsWith(path, ".css")) return "text/css";
    if (endsWith(path, ".js")) return "application/javascript";
    if (endsWith(path, ".png")) return "image/png";
    if (endsWith(path, ".jpg") || endsWith(path, ".jpeg")) return "image/jpeg";
    if (endsWith(path, ".gif")) return "image/gif";
    if (endsWith(path, ".svg")) return "image/svg+xml";
    if (endsWith(path, ".ico")) return "image/x-icon";
    return "application/octet-stream"; // fallback
}

#endif
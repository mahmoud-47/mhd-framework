#include "../../settings.hpp"
#include "HttpRender.hpp"

// read html file
std::string readHtmlFile(const std::string& filepath) {
    std::string compelete_file_path = TEMPLATES_DIR + "/" + filepath;
    // std::cout << compelete_file_path << std::endl;
    std::ifstream file(compelete_file_path);
    if (!file.is_open()) {
        return "<html><body><h1>Error: File not found</h1></body></html>";
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    return ss.str();
}

// Has to take context after
void renderHtml(Request request ,const std::string& filepath){
    std::string http_response =
                "HTTP/1.1 200 OK\r\n" 
                "Content-Type: text/html\r\n" 
                "Connection: close\r\n" 
                "\r\n" + readHtmlFile(filepath);
    send(request.getSocket(), http_response.c_str(), http_response.length(), 0);
    close(request.getSocket());
}

// Render text
void renderText(Request request ,const std::string& text){
    std::string http_response =
                "HTTP/1.1 200 OK\r\n" 
                "Content-Type: text/html\r\n" 
                "Connection: close\r\n" 
                "\r\n" + text;
    send(request.getSocket(), http_response.c_str(), http_response.length(), 0);
    close(request.getSocket());
}
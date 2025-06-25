#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <fstream>
#include <sstream>
#include <random>

#include "settings.hpp"
#include "utils/request/Request.hpp"
#include "utils/route/routes.hpp"
#include "utils/URLPattern.hpp"
#include "urls/urls.hpp"
#include "utils/functions.hpp"
#include "Models/Migration/migrations.hpp"
#include "utils/nice-display/display.hpp"

std::string readFullRequest(int client_socket) {
    std::string fullRequest;
    char buffer[4096];
    ssize_t bytesRead;
    
    // Read the headers first
    std::string headers;
    bool headersComplete = false;
    
    while (!headersComplete) {
        bytesRead = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
        if (bytesRead <= 0) break;
        
        buffer[bytesRead] = '\0';
        headers += std::string(buffer, bytesRead);  // Use bytesRead to handle binary data
        
        // Check if headers are complete (look for \r\n\r\n)
        size_t headerEnd = headers.find("\r\n\r\n");
        if (headerEnd != std::string::npos) {
            headersComplete = true;
            fullRequest = headers.substr(0, headerEnd + 4);
            
            // Check if there's any body data already read
            if (headers.length() > headerEnd + 4) {
                fullRequest += headers.substr(headerEnd + 4);
            }
            break;
        }
    }
    
    // Parse Content-Length from headers
    size_t contentLengthPos = fullRequest.find("Content-Length:");
    if (contentLengthPos != std::string::npos) {
        size_t start = fullRequest.find(":", contentLengthPos) + 1;
        size_t end = fullRequest.find("\r\n", start);
        std::string lengthStr = fullRequest.substr(start, end - start);
        
        // Remove whitespace
        lengthStr.erase(0, lengthStr.find_first_not_of(" \t"));
        lengthStr.erase(lengthStr.find_last_not_of(" \t") + 1);
        
        int contentLength = std::stoi(lengthStr);
        size_t headerEnd = fullRequest.find("\r\n\r\n");
        int bodyBytesAlreadyRead = fullRequest.length() - (headerEnd + 4);
        int remainingBytes = contentLength - bodyBytesAlreadyRead;
        
        // std::cout << "Content-Length: " << contentLength << std::endl;
        // std::cout << "Body bytes already read: " << bodyBytesAlreadyRead << std::endl;
        // std::cout << "Remaining bytes to read: " << remainingBytes << std::endl;
        
        // Read remaining body data
        while (remainingBytes > 0) {
            int toRead = std::min(remainingBytes, (int)sizeof(buffer));
            bytesRead = recv(client_socket, buffer, toRead, 0);
            if (bytesRead <= 0) break;
            
            fullRequest += std::string(buffer, bytesRead);  // Use bytesRead for binary data
            remainingBytes -= bytesRead;
            
            // std::cout << "Read " << bytesRead << " bytes, " << remainingBytes << " remaining" << std::endl;
        }
    }
    
    return fullRequest;
}

void handle_connection(int client_socket, Route* routes) {
    std::string requestData = readFullRequest(client_socket);

    /**
     * SET session_id and csrf_token if not exists
     */
    Request request(requestData);
    Session session(request);
    if(session.get_value("csrf_token") == "")
        session.set_value("csrf_token", generateRandomToken());
    
    // std::cout << "SMTP_SENDER_EMAIL " << SMTP_SENDER_EMAIL << std::endl;
    // std::cout << "SMTP_SENDER_PASSWORD " << SMTP_SENDER_PASSWORD << std::endl;

    std::string url = request.getUrl();
    

    if (url.find("/static/") == 0) {
        std::string relativePath = url.substr(8); // remove "/static/"
        std::string filePath = STATIC_DIR + "/" + relativePath;
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            const char *notFound =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Connection: close\r\n\r\n"
                "Static file not found.";
            send(client_socket, notFound, strlen(notFound), 0);
        } else {
            std::ostringstream oss;
            oss << file.rdbuf();
            std::string content = oss.str();
            std::string header =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: " + getMimeType(filePath) + "\r\n"
                "Content-Length: " + std::to_string(content.size()) + "\r\n"
                "Connection: close\r\n\r\n";

            std::cout << "\033[32m[STATIC]\033[0m Served \033[36m/" << relativePath
          << "\033[0m (" << getMimeType(filePath) << ", "
          << content.size() << " bytes)" << std::endl;
            send(client_socket, header.c_str(), header.size(), 0);
            send(client_socket, content.c_str(), content.size(), 0);
        }

        close(client_socket);
        return;
    } else {
        URLPattern* urlpattern = routes->getUrlPattern(request.getUrl());

        if ((request.clientWantAnything() || request.clientWantsHtml()) && urlpattern) {
            request.setUrlFormat(urlpattern->url);
            request.setSocket(client_socket);

            // check if the csrf_token is correct before calling any controller in case this bro is posting
            if (request.getMethod() == "POST" || request.getMethod() == "PUT" || request.getMethod() == "DELETE") {
                std::string submitted = request.getFormDataParameterByParameterName("csrf_token");
                std::string expected = session.get_value("csrf_token");

                if (submitted.empty() || submitted != expected) {
                    renderText(request, "CSRF token is missing or invalid");
                    return;
                }
            }

            MhdDateTime now;
            std::string console_out = "\n\033[36m" + now.to_date_string() + " " + now.to_time_string() + "\033[0m \033[33m[REQUEST LOG]\033[0m\n";
            console_out += "├─ \033[32mHostname\033[0m     : " + request.getHostName() + "\n";
            console_out += "├─ \033[32mMethod\033[0m       : " + request.getMethod() + "\n";
            console_out += "├─ \033[32mUser Agent\033[0m   : " + request.getUserAgent() + "\n";
            console_out += "└─ \033[32mURL\033[0m          : " + request.getUrl() + "\n\n";


            std::cout << console_out;


            urlpattern->controller(request);
        } else if (request.clientWantsHtml()) {
            const char *http_response_404 =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<html><body><h1>404 - Page Not Found</h1></body></html>";
            send(client_socket, http_response_404, strlen(http_response_404), 0);
        } else {
            const char *http_response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<html><body><h1>Bonjour depuis un serveur HTTP C++ !</h1></body></html>";
            send(client_socket, http_response, strlen(http_response), 0);
        }
    }

    close(client_socket);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 10) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "**********************************************" << std::endl;
    std::cout << "WELCOME TO MHD++ Version " << VERSION << "..." << std::endl;
    std::cout << "**********************************************" << std::endl << std::endl;

    display_banner();

    std::cout << "Server Listening on port " << PORT << "..." << std::endl;
    std::cout << "Base dir = " << getBaseDirectory() << std::endl;
    std::cout << "Templates dir = " << TEMPLATES_DIR << std::endl;
    std::cout << "Static dir = " << STATIC_DIR << std::endl;

    initialize_tables();

    Route *routes = new Route;
    for (int i = 0; i < sizeof(urls) / sizeof(URLPattern); i++)
        routes->addRoute(urls[i]);

    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("Erreur accept");
            continue;
        }

        // Start a new thread for each connection
        std::thread t(handle_connection, new_socket, routes);
        t.detach(); // Do not wait for the thread to finish
    }

    close(server_fd);
    delete routes;

    return 0;
}

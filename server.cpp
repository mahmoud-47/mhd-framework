#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <fstream>
#include <sstream>

#include "settings.hpp"
#include "utils/request/Request.hpp"
#include "utils/route/routes.hpp"
#include "utils/URLPattern.hpp"
#include "urls/urls.hpp"
#include "utils/functions.hpp"
#include "Models/Migration/migrations.hpp"

void handle_connection(int client_socket, Route* routes) {
    char buffer[3000] = {0};
    read(client_socket, buffer, 3000);
    std::cout << "Requête reçue:\n" << buffer << std::endl;

    Request request(buffer);
    std::cout << "Request Hostname " << request.getHostName() << std::endl;
    std::cout << "Request method " << request.getMethod() << std::endl;
    std::cout << "Request userAgent " << request.getUserAgent() << std::endl;
    std::cout << "Request Url " << request.getUrl() << std::endl;

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

            send(client_socket, header.c_str(), header.size(), 0);
            send(client_socket, content.c_str(), content.size(), 0);
        }

        close(client_socket);
        return;
    }

    URLPattern* urlpattern = routes->getUrlPattern(request.getUrl());

    if ((request.clientWantAnything() || request.clientWantsHtml()) && urlpattern) {
        request.setUrlFormat(urlpattern->url);
        request.setSocket(client_socket);
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

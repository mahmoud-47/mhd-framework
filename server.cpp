#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "settings.hpp"
#include "utils/request/Request.hpp"
#include "utils/route/routes.hpp"
#include "utils/URLPattern.hpp"
#include "urls/urls.hpp"
#include "utils/functions.hpp"
#include "Models/Migration/migrations.hpp"

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    const char *http_response =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n"
        "Connection: close\r\n"
        "\r\n"
        "<html><body><h1>Bonjour depuis un serveur HTTP C++ !</h1></body></html>";

    // Création du socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Erreur socket");
        exit(EXIT_FAILURE);
    }

    // Allow socket to reuse address
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Erreur bind");
        exit(EXIT_FAILURE);
    }

    // Écoute
    if (listen(server_fd, 10) < 0) {
        perror("Erreur listen");
        exit(EXIT_FAILURE);
    }

    std::cout << "Serveur HTTP en écoute sur le port " << PORT << "..." << std::endl;
    std::cout << "Base dir = " << getBaseDirectory() << std::endl;
    std::cout << "Templates dir = " << TEMPLATES_DIR << std::endl;
    std::cout << "Static dir = " << STATIC_DIR << std::endl;

    /************* Migrations */
    initialize_tables();

    /************* Route */
    Route *routes = new Route;
    // Add URL patterns to the routes
    for(int i = 0; i < sizeof(urls)/sizeof(URLPattern); i++)
        routes->addRoute(urls[i]);
    // std::cout << "Route name " << urls[0].name << std::endl;
    // std::cout << "Route url " << urls[0].url << std::endl;
    // std::cout << "size Route url " << sizeof(urls)/sizeof(URLPattern) << std::endl;


    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
        if (new_socket < 0) {
            perror("Erreur accept");
            continue;
        }

        char buffer[3000] = {0};
        read(new_socket, buffer, 3000);
        std::cout << "Requête reçue:\n" << buffer << std::endl;
        
        /* Request */
        Request request(buffer);
        std::cout << "Request Hostname " << request.getHostName() << std::endl;
        std::cout << "Request method " << request.getMethod() << std::endl;
        std::cout << "Request userAgent " << request.getUserAgent() << std::endl;
        std::cout << "Request Url " << request.getUrl() << std::endl;

        std::string url = request.getUrl();

        if (url.find("/static/") == 0) {
            std::cout << "Get static" << std::endl;
            std::string relativePath = url.substr(8); // remove "/static/"
            std::string filePath = STATIC_DIR + "/" + relativePath;
            std::cout << "file path static" << filePath << std::endl;

            std::ifstream file(filePath, std::ios::binary);
            if (!file) {
                const char *notFound =
                    "HTTP/1.1 404 Not Found\r\n"
                    "Content-Type: text/plain\r\n"
                    "Connection: close\r\n\r\n"
                    "Static file not found.";
                send(new_socket, notFound, strlen(notFound), 0);
            } else {
                std::ostringstream oss;
                oss << file.rdbuf();
                std::string content = oss.str();

                std::string header =
                    "HTTP/1.1 200 OK\r\n"
                    "Content-Type: " + getMimeType(filePath) + "\r\n"
                    "Content-Length: " + std::to_string(content.size()) + "\r\n"
                    "Connection: close\r\n\r\n";

                send(new_socket, header.c_str(), header.size(), 0);
                send(new_socket, content.c_str(), content.size(), 0);
            }

            close(new_socket);
            continue; // Skip the rest of the loop
        }
        
        // check if the url exixts
        URLPattern *urlpattern = routes->getUrlPattern(request.getUrl());

        if((request.clientWantAnything() || request.clientWantsHtml()) && urlpattern){
            std::cout << "URL found" << std::endl;
            request.setUrlFormat(urlpattern->url);
            request.setSocket(new_socket);
            urlpattern->controller(request);
            continue;
        // }else if(request.clientWantsHtml()){
        }else if(request.clientWantsHtml()){
            std::cout << "URL not found" << std::endl;
            const char *http_response_404 =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<html><body><h1>404 - Page Not Found</h1></body></html>";
                send(new_socket, http_response_404, strlen(http_response_404), 0);
                close(new_socket);
                continue;
        }else{
            std::cout << "Not HTML resource" << std::endl;
        }


        send(new_socket, http_response, strlen(http_response), 0);
        close(new_socket);
        // free the allocated memory
        delete urlpattern;
    }

    close(server_fd);
    // free the allocated memory
    delete routes;
    
    return 0;
}
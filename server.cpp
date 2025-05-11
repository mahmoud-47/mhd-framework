#include <iostream>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "settings.hpp"
#include "utils/request/Request.hpp"
#include "router/routes.hpp"
#include "utils/URLPattern.hpp"

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
    std::cout << "Base dir = " << BASE_DIRECTORY << std::endl;
    std::cout << "Templates dir = " << TEMPLATES_DIR << std::endl;
    std::cout << "Static dir = " << STATIC_DIR << std::endl;

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
        
        URLPattern *urlpattern = Route::getUrlPattern(request.getUrl());
        if(1==2){
            const char *http_response_404 =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n"
                "Connection: close\r\n"
                "\r\n"
                "<html><body><h1>404 - Page Not Found</h1></body></html>";
                send(new_socket, http_response_404, strlen(http_response_404), 0);
        }

        send(new_socket, http_response, strlen(http_response), 0);
        close(new_socket);
    }

    close(server_fd);
    return 0;
}
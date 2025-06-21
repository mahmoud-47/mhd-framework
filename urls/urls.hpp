#include <vector>
#include "utils/URLPattern.hpp"
#include "Controllers/HelloController.hpp"
#include "Controllers/UserController.hpp"
#include "Controllers/ClientController.hpp"

// Url format = {"name", "url/{id}", HelloController::Controller}
URLPattern urls[] = {
    // Hello URLS
    {"hello", "/", HelloController::Hello},
    {"documentation", "/documentation", HelloController::Documentation},
    {"string", "/string/{id}", HelloController::HelloString},
    {"parse", "/test-parse", HelloController::TestParser},

    // User URLS
    {"register", "/user/register", UserController::regirer},
    {"login", "/user/login", UserController::login},
    

    // Client URLS
    {"display-all", "/clients/all", ClientController::DisplayAll},
    {"create-client", "/clients/create", ClientController::Create},
    {"edit-client", "/clients/edit/{id}", ClientController::Update},
    {"detail-client", "/clients/details/{id}", ClientController::Details},
    {"delete-client", "/clients/delete/{id}", ClientController::Delete}
};
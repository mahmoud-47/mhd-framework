#include <vector>
#include "../utils/URLPattern.hpp"
#include "Controllers/HelloController.hpp"
#include "Controllers/UserController.hpp"
#include "Controllers/ClientController.hpp"
#include "../utils/filter/filters.hpp" // Filters (AuthenticatedUsersOnly and UnAuthenticatedUsersOnly)
#include "../utils/render/HttpRender.hpp" // NOT needed but as i wanted to render the hello page directly form here, I added it

// Url format = {"name", "url/{id}", HelloController::Controller}
URLPattern urls[] = {
    // Hello URLS (No filtering have been applied to these urls)
    {"hello", "/", HelloController::Hello},
    {"documentation", "/documentation", HelloController::Documentation},

    // User URLS
    // must be unauthenticated
    {"register", "/user/register", UnAuthenticatedUsersOnly(UserController::regirer, "/clients/all")},
    {"login", "/user/login", UnAuthenticatedUsersOnly(UserController::login, "/clients/all")},
    // only authenticated users can logout
    {"logout", "/user/logout", AuthenticatedUsersOnly(UserController::logout_user, "/user/login")},

    // Client URLS (protect them all : user needs to be authenticated)
    {"display-all", "/clients/all", AuthenticatedUsersOnly(ClientController::DisplayAll, "/user/login")},
    {"create-client", "/clients/create", AuthenticatedUsersOnly(ClientController::Create, "/user/login")},
    {"edit-client", "/clients/edit/{id}", AuthenticatedUsersOnly(ClientController::Update, "/user/login")},
    {"detail-client", "/clients/details/{id}", AuthenticatedUsersOnly(ClientController::Details, "/user/login")},
    {"delete-client", "/clients/delete/{id}", AuthenticatedUsersOnly(ClientController::Delete, "/user/login")}
};

/*
You could call a controller without any Controller class

{"hello", "/", [](Request& request){
    std::string templatename = "hello.html";
    Context context;
    renderHtml(request, templatename, context);
}}

*/
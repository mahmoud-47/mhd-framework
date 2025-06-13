#include <vector>
#include "utils/URLPattern.hpp"
#include "Controllers/HelloController.hpp"
#include "Controllers/UserController.hpp"

// Url format = {"name", "url/{id}", HelloController::Controller}
URLPattern urls[] = {
    // Hello URLS
    {"hello", "/", HelloController::Hello},
    {"documentation", "/documentation", HelloController::Documentation},
    {"string", "/string/{id}", HelloController::HelloString},
    {"parse", "/test-parse", HelloController::TestParser},

    // User URLS
    {"adduser", "/adduser", UserController::createUser},
    {"getuser", "/getuser/{userId}", UserController::getUser},
    {"getusers", "/getusers", UserController::getAllUsers},
    {"deluser", "/deluser/{userId}", UserController::deluser},
    {"updateUser", "/updateuser/{userId}", UserController::updateUser}
};
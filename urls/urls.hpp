#include <vector>
#include "utils/URLPattern.hpp"
#include "Controllers/HelloController.hpp"

// Url format = {"name", "url/{id}", HelloController::Controller}
URLPattern urls[] = {
    {"hello", "/", HelloController::Hello},
    {"documentation", "/documentation", HelloController::Documentation},
    {"string", "/string/{id}", HelloController::HelloString}
};
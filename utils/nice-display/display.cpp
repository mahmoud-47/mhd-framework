#include "display.hpp"
#include <iostream>
#include <fstream>
#include <string>

// #include <unistd.h>
// #include <limits.h>
// #include <iostream>

// void debug_pwd() {
//     char cwd[PATH_MAX];
//     if (getcwd(cwd, sizeof(cwd)) != nullptr) {
//         std::cout << "[DEBUG] Current working directory: " << cwd << std::endl;
//     } else {
//         perror("getcwd() error");
//     }
// }

void display_banner() {
    std::ifstream file("../utils/nice-display/mhd++.txt"); 
    // debug_pwd(); 

    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open mhd++.txt" << std::endl;
        return;
    }

    const std::string yellow = "\033[1;32m";  // Bright green
    const std::string reset  = "\033[0m";    // Reset to default color

    std::string line;
    while (std::getline(file, line)) {
        std::cout << yellow << line << reset << std::endl;
    }

    file.close();
}

#include "current_path.hpp"
#include <iostream>
#include <limits.h>
#include <filesystem>
#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define getCurrDirectory_ getcwd
#endif

std::string getBaseDirectory() {
   
    // Use __FILE__ to get the current file's path
    std::filesystem::path path = std::filesystem::absolute(__FILE__);
    
    // Now, navigate to the directory containing settings.hpp
    path = path.parent_path().parent_path().parent_path(); 

    return path.string(); // Return the path as a string
}


#ifndef __MHD_FRAM_CURRENT_PATH__
#define __MHD_FRAM_CURRENT_PATH__

    #include <iostream>
    #include <limits.h>
    #ifdef _WIN32
    #include <direct.h>
    #define GetCurrentDir _getcwd
    #else
    #include <unistd.h>
    #define GetCurrentDir getcwd
    #endif

    #include<cstring>


    std::string getCurrentDir() {
        char currentPath[PATH_MAX];
        if (GetCurrentDir(currentPath, sizeof(currentPath)) != nullptr) {
            std::cout << "Current working directory: " << currentPath << std::endl;
        } else {
            perror("GetCurrentDir");
            return nullptr;
        }
        return currentPath;
    }

    const std::string BASE_DIRECTORY = getCurrentDir();

#endif



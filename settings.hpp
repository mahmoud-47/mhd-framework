#ifndef __MHD_FRAM_SETT__
#define __MHD_FRAM_SETT__

    /**
     * These Constants are vital for the program, so don't delete them, You can modify them..
     */

    #include "utils/current_path.hpp"

    // Port
    #define PORT 8000

    // Templates DIR 
    const std::string TEMPLATES_DIR = BASE_DIRECTORY + "/templates";

    // Static Files Folder
    const std::string STATIC_DIR = TEMPLATES_DIR + "/static";

    /* DATABASE CONFIG*/

    // SQLITE
    const std::string DATABASE_PATH = BASE_DIRECTORY + "/database/db.sqlite";
    

#endif
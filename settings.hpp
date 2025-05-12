#ifndef __MHD_FRAM_SETT__
#define __MHD_FRAM_SETT__

    /**
     * These Constants are vital for the program, so don't delete them, You can modify them..
     */

    #include "utils/current_path/current_path.hpp"

    // Port
    #define PORT 8001

    // Templates DIR 
    const std::string TEMPLATES_DIR = getBaseDirectory() + "/templates";

    // Static Files Folder
    const std::string STATIC_DIR = TEMPLATES_DIR + "/static";

    /* DATABASE CONFIG*/

    // SQLITE
    const std::string DATABASE_PATH = getBaseDirectory() + "/database/db.sqlite";
    

#endif
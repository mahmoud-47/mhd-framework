#ifndef __MHD_FRAM_SETT__
#define __MHD_FRAM_SETT__

    /**
     * These Constants are vital for the program, so don't delete them, You can modify them..
     */

    #include "utils/current_path/current_path.hpp"

    // Port
    #define PORT 8001

    // Base directory
    const std::string BASE_DIR = getBaseDirectory();

    // Templates DIR 
    const std::string TEMPLATES_DIR = getBaseDirectory() + "/templates";

    // Static Files Folder
    const std::string STATIC_DIR = TEMPLATES_DIR + "/static";

    // Uploaded Files Folder
    const std::string UPLOADS_DIR = STATIC_DIR + "/uploads";

    /* DATABASE CONFIG*/

    // SQLITE
    const std::string DATABASE_PATH = getBaseDirectory() + "/database/db.sqlite";

    // SHOW SQL queries
    const bool showSQLQueries = true;

    // DROP SQL TABLE AND Recreate them
    const bool dropRecreateTables = false;

    // Version
    #define VERSION "1.1"
    

#endif
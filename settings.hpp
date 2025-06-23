#ifndef __MHD_FRAM_SETT__
#define __MHD_FRAM_SETT__

    /**
     * These Constants are vital for the program, so don't delete them, You can modify them..
     */

    #include "utils/current_path/current_path.hpp"

    // Port
    #define PORT 8001

    // Base directory containing settings.hpp
    const std::string BASE_DIR = getBaseDirectory();

    // HTML templates location
    const std::string TEMPLATES_DIR = getBaseDirectory() + "/templates";

    // Static files location
    const std::string STATIC_DIR = TEMPLATES_DIR + "/static";

    // Upload directory
    const std::string UPLOADS_DIR = STATIC_DIR + "/uploads";

    /* DATABASE CONFIG*/

    // SQLite database path
    const std::string DATABASE_PATH = getBaseDirectory() + "/database/db.sqlite";

    // Show SQL queries in console
    const bool showSQLQueries = true;

    // Drop and recreate tables on startup
    const bool dropRecreateTables = false;

    // MHD++ Version
    #define VERSION "1.1"
    

#endif
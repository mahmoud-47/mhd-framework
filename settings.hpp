#ifndef __MHD_FRAM_SETT__
#define __MHD_FRAM_SETT__

    /**
     * These Constants are vital for the program, so don't delete them, You can modify them..
     */

    #include "utils/current_path/current_path.hpp"
    #include "utils/load_env/load_env.hpp"

    // Secret key (generated with openssl rand -hex 32)
    // We advice you to generate a new secret key by running the linux command "openssl rand -hex 32"
    // it's even better to save it in the .env file and load it after
    const std::string SECRET_KEY = "cbf843449b0b0a2c98963206d83a232aae8aa32977844f74486c9e2a76248e96";

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
    const bool showSQLQueries = false;

    // Drop and recreate tables on startup
    const bool dropRecreateTables = false;

    // MHD++ Version
    #define VERSION "1.1"

    /**
     * SMTP Settings
     */
    
    // create a .env file in the same directory and store your email and app password there the file shoul look like this
    /*
    SENDER_EMAIL=xyz@gmail.com
    SENDER_PASSWORD=abcd abcd abcd abcd
    */
    const std::string SMTP_SERVER = "smtp://smtp.gmail.com:587";
    const std::string SMTP_SENDER_EMAIL = getenv_from_env_file("SENDER_EMAIL");
    const std::string SMTP_SENDER_PASSWORD = getenv_from_env_file("SENDER_PASSWORD");
    

#endif
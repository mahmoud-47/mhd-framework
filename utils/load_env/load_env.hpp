#ifndef __LOAD_ENV_
#define __LOAD_ENV_

    #include <iostream>
    #include <fstream>
    #include <string>
    #include <unordered_map>
    #include <sstream>
    #include <algorithm>

   
    /**
     * @brief This is going to fetch the value stored in the .env file by the key
     * @param key is the key of the hidden value
     * @param filepath is the path of the hidden .env file, (default if not given = ../.env)
     * @return returns the value of the key, or "" if not found
     */
    std::string getenv_from_env_file(const std::string& key, const std::string& filepath = "../.env");

#endif
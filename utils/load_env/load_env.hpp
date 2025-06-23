#ifndef __LOAD_ENV_
#define __LOAD_ENV_

    #include <iostream>
    #include <fstream>
    #include <string>
    #include <unordered_map>
    #include <sstream>
    #include <algorithm>

    std::string trim(const std::string& str);

    std::unordered_map<std::string, std::string> load_env_file(const std::string& filepath = "../.env");

    std::string getenv_from_env_file(const std::string& key, const std::string& filepath = "../.env");

#endif
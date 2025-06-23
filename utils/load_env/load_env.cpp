#include "load_env.hpp"

std::string trim(const std::string& str) {
    const char* whitespace = " \t\n\r\f\v";
    size_t start = str.find_first_not_of(whitespace);
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(whitespace);
    return str.substr(start, end - start + 1);
}

std::unordered_map<std::string, std::string> load_env_file(const std::string& filepath) {
    std::unordered_map<std::string, std::string> env_map;
    std::ifstream file(filepath);

    if(!file.is_open()){
        std::cout << "Cannot open .env " << std::endl;
    }
    std::string line;

    while (std::getline(file, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t eq_pos = line.find('=');
        if (eq_pos == std::string::npos) continue;

        std::string key = trim(line.substr(0, eq_pos));
        std::string value = trim(line.substr(eq_pos + 1));
        env_map[key] = value;
    }

    return env_map;
}

std::string getenv_from_env_file(const std::string& key, const std::string& filepath) {
    static std::unordered_map<std::string, std::string> env_cache = load_env_file(filepath);
    auto it = env_cache.find(key);
    if (it != env_cache.end()) {
        return it->second;
    }
    return ""; 
}
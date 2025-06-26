#ifndef __MHD_SESSION__
#define __MHD_SESSION__

#include <string>
#include <random>
#include "../nlohmann_json/json.hpp"
#include "../crypto/use_monocypher.hpp"

class Request;
class SessionTable;

using Json = nlohmann::json;

class Session {
private:
    std::string session_id;
    Json content;

    std::string generateSessionID(size_t length = 32);
    std::string generateUniqueSessionID();
    void updateDatabase();

public:
    Session(Request& request);

    template<typename T>
    T get_value(const std::string& key) const;

    template<typename T>
    void set_value(const std::string& key, const T& value);

    void remove_key(const std::string& key);
    const std::string& get_session_id() const { return session_id; }
};

// Template definitions
template<typename T>
T Session::get_value(const std::string& key) const {
    if (content.contains(key)) {
        try {
            return content.at(key).get<T>();
        } catch (...) {
            return T{};
        }
    }
    return T{};
}

template<typename T>
void Session::set_value(const std::string& key, const T& value) {
    content[key] = value;
    updateDatabase();
}

#endif

#include "session.hpp"
#include "../../Models/session.hpp"
#include "../request/Request.hpp"

Session::Session(Request& request) {
    if (request.get_cookie("session_id").size() > 0) {
        session_id = request.get_cookie("session_id");
        SessionTable sessionQuery;
        auto session_query = sessionQuery.find_by("session_id", session_id);

        if (session_query.empty()) {
            session_id = generateUniqueSessionID();
            content = Json::object();
        } else {
            session_id = static_cast<SessionTable*>(session_query[0])->session_id;
            std::string stored_base64 = static_cast<SessionTable*>(session_query[0])->content;

            try {
                std::string encrypted = base64::decode(stored_base64);
                std::string decrypted = decrypt_message(encrypted, SECRET_KEY);
                content = Json::parse(decrypted);
            } catch (...) {
                content = Json::object(); // fallback to empty
            }
        }
    } else {
        session_id = generateUniqueSessionID();
        content = Json::object();
    }

    request.set_session_id(session_id);
}

void Session::updateDatabase() {
    SessionTable sessionQuery;
    auto session_query = sessionQuery.find_by("session_id", session_id);

    if (!session_query.empty()) {
        int id = static_cast<SessionTable*>(session_query[0])->id;

        std::string json_str = content.dump();
        std::string encrypted = crypt_message(json_str, SECRET_KEY);
        std::string base64_encoded = base64::encode(std::string(encrypted.data(), encrypted.size()));

        static_cast<SessionTable*>(session_query[0])->content = base64_encoded;
        static_cast<SessionTable*>(session_query[0])->update(std::to_string(id));
    }
}

void Session::remove_key(const std::string& key) {
    if (content.contains(key)) {
        content.erase(key);
        updateDatabase();
    }
}

std::string Session::generateSessionID(size_t length) {
    static const char charset[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2);
    std::string sessionID;
    sessionID.reserve(length);
    for (size_t i = 0; i < length; ++i)
        sessionID += charset[dist(generator)];
    return sessionID;
}

std::string Session::generateUniqueSessionID() {
    SessionTable sessionQuery;
    while (true) {
        std::string new_id = generateSessionID();
        auto result = sessionQuery.find_by("session_id", new_id);
        if (result.empty()) {
            Json initial_content = Json::object();
            std::string json_str = initial_content.dump();
            std::string encrypted = crypt_message(json_str, SECRET_KEY);
            std::string base64_encoded = base64::encode(std::string(encrypted.data(), encrypted.size()));

            SessionTable session(0, new_id, base64_encoded);
            session.save();
            return new_id;
        }
    }
}
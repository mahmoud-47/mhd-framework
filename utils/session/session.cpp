#include "session.hpp"
#include "../../Models/session.hpp"
#include "../request/Request.hpp"


// Session constructor
Session::Session(Request& request){
    if(request.get_cookie("session_id").size() > 0){
        session_id = request.get_cookie("session_id");
        // check if this session is in the table
        SessionTable sessionQuery;
        auto session_query = sessionQuery.find_by("session_id", session_id);
        if(session_query.size() == 0){ 
            // there is a session id from the cookies but not in the db
            // create a new session and store it
            session_id = generateUniqueSessionID();
            content = "";
        }else{
            // the session from the cookie exists in the db
            session_id = static_cast<SessionTable*>(session_query[0])->session_id;
            content = static_cast<SessionTable*>(session_query[0])->value;
        }
    }else{
        // No cookie with session_id
        session_id = generateUniqueSessionID();
        content = "";
    }
    // update the request session_id
    request.set_session_id(session_id);
}   

// Get the value of a key, returns "" if not found
std::string Session::get_value(const std::string key) {
    std::istringstream ss(content);
    std::string pair;

    while (std::getline(ss, pair, ';')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string k = pair.substr(0, pos);
            std::string v = pair.substr(pos + 1);
            if (k == key) {
                return v;
            }
        }
    }
    return "";
}

// Set value (key, value)
void Session::set_value(const std::string key, const std::string value) {
    std::istringstream ss(content);
    std::ostringstream new_content;
    std::string pair;
    bool found = false;

    while (std::getline(ss, pair, ';')) {
        size_t pos = pair.find('=');
        if (pos != std::string::npos) {
            std::string k = pair.substr(0, pos);
            std::string v = pair.substr(pos + 1);
            if (k == key) {
                new_content << key << "=" << value << ";";
                found = true;
            } else {
                new_content << k << "=" << v << ";";
            }
        }
    }

    if (!found) {
        new_content << key << "=" << value << ";";
    }

    content = new_content.str();

    // update in the db
    SessionTable sessionQuery;
    auto session_query = sessionQuery.find_by("session_id", session_id);
    if(session_query.size() > 0){
        int id = static_cast<SessionTable*>(session_query[0])->id;
        static_cast<SessionTable*>(session_query[0])->value = content;
        static_cast<SessionTable*>(session_query[0])->update(std::to_string(id));
    }
}

// Helper guy
std::string Session::generateSessionID(size_t length) {
    static const char charset[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::random_device rd;  // Cryptographically secure if available
    std::mt19937 generator(rd());
    std::uniform_int_distribution<> dist(0, sizeof(charset) - 2); // -2 to exclude null terminator

    std::string sessionID;
    sessionID.reserve(length);

    for (size_t i = 0; i < length; ++i) {
        sessionID += charset[dist(generator)];
    }

    return sessionID;
}

// Helper guy 2
std::string Session::generateUniqueSessionID(){
    SessionTable sessionQuery;
    while(true){
        std::string session_id = generateSessionID();
        auto session_query = sessionQuery.find_by("session_id", session_id);
        // if the generated id does not exist, we good !
        if(session_query.size() == 0){
            // save the session to the db
            SessionTable session(0, session_id, "");
            session.save();
            return session_id;
        }
    }
}
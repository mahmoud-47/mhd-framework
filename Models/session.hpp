#ifndef __SESSION_MODEL_H__
#define __SESSION_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"  

class Session : public SQLiteORM {
public:
    std::string session_id;
    std::string key;
    std::string value;

    void registerFields(){
        fields.clear();

        fields.push_back({"id", "TEXT", &session_id, ""});
        fields.push_back({"key", "TEXT", &key, ""});
        fields.push_back({"value", "TEXT", &value, ""});
    }

    Session() : SQLiteORM("sessions") {
        registerFields();
    };

    Session(const std::string& session_id, const std::string& key, const std::string& value) : SQLiteORM("sessions") {
        this->session_id = session_id;
        this->key = key;
        this->value = value;
        registerFields();
    }

    // Proper copy constructor
    Session(const Session& other) : SQLiteORM(other), 
                            session_id(other.session_id),
                            key(other.key),
                            value(other.value) {
        registerFields();  // This updates the pointers to our new members
    }

    // Virtual destructor
    virtual ~Session() = default;

    ORMModel* clone() const override {
        return new Session(*this);
    }

};

#endif

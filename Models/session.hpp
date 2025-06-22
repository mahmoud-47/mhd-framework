#ifndef __SESSION_MODEL_H__
#define __SESSION_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"  

class SessionTable : public SQLiteORM {
public:
    int id;
    std::string session_id;
    std::string value;

    void registerFields(){
        fields.clear();

        fields.push_back({"id", "INTEGER", &id, "PRIMARY KEY"});
        fields.push_back({"session_id", "TEXT", &session_id, "UNIQUE"});
        fields.push_back({"value", "TEXT", &value, ""});
    }

    SessionTable() : SQLiteORM("sessions") {
        registerFields();
    };

    SessionTable(int id, const std::string& session_id, const std::string& value) : SQLiteORM("sessions") {
        this->id = id;
        this->session_id = session_id;
        this->value = value;
        registerFields();
    }

    // Proper copy constructor
    SessionTable(const SessionTable& other) : SQLiteORM(other), 
                            id(other.id),
                            session_id(other.session_id),
                            value(other.value) {
        registerFields();  // This updates the pointers to our new members
    }

    // Virtual destructor
    virtual ~SessionTable() = default;

    ORMModel* clone() const override {
        return new SessionTable(*this);
    }

};

#endif

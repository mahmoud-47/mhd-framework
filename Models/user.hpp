#ifndef __USER_MODEL_H__
#define __USER_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"  

class User : public SQLiteORM {
public:
    int id;
    std::string name;
    std::string email;

    // Getter for id
    int getId() const {
        return id;
    }

    // Setter for id
    void setId(int newId) {
        id = newId;
    }

    // Getter for name
    std::string getName() const {
        return name;
    }

    // Setter for name
    void setName(const std::string& newName) {
        name = newName;
    }

    // Getter for email
    std::string getEmail() const {
        return email;
    }

    // Setter for email
    void setEmail(const std::string& newEmail) {
        email = newEmail;
    }

    void registerFields(){
        fields.clear();

        fields.push_back({"id", "INTEGER", &id, "PRIMARY KEY"});
        fields.push_back({"name", "TEXT", &name, ""});
        fields.push_back({"email", "TEXT", &email, "UNIQUE"});
    }

    User() : SQLiteORM("users") {
        registerFields();
    };

    User(const int id, const std::string& name, const std::string& email) : SQLiteORM("users") {
        this->id = id;
        this->name = name;
        this->email = email;
        registerFields();
    }
};

#endif

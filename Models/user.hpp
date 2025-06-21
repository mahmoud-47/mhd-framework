#ifndef __USER_MODEL_H__
#define __USER_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"  

class User : public SQLiteORM {
public:
    int id;
    std::string name;
    std::string email;
    std::string username;
    std::string password;


    void registerFields(){
        fields.clear();
        fields.push_back({"id", "INTEGER", &id, "PRIMARY KEY"});
        fields.push_back({"name", "VARCHAR(50)", &name, ""});
        fields.push_back({"email", "VARCHAR(100)", &email, "UNIQUE"});
        fields.push_back({"username", "VARCHAR(100)", &username, "UNIQUE"});
        fields.push_back({"password", "VARCHAR(255)", &password});
    }

    User() : SQLiteORM("users") {
        registerFields();
    }

    // Proper copy constructor
    User(const User& other) : SQLiteORM(other), 
                            id(other.id),
                            name(other.name),
                            email(other.email) {
        registerFields();  // This updates the pointers to our new members
    }

    // Parameterized constructor
    User(const int id, const std::string& name, const std::string& email) : 
        SQLiteORM("users"), id(id), name(name), email(email) {
        registerFields();
    }

    // Virtual destructor
    virtual ~User() = default;

    ORMModel* clone() const override {
        return new User(*this);
    }

};

#endif
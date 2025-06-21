#ifndef __USER_MODEL_H__
#define __USER_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"  

class User : public SQLiteORM {
public:
    int id;
    std::string firstname;
    std::string lastname;
    std::string email;
    std::string username;
    std::string password;


    void registerFields(){
        fields.clear();
        fields.push_back({"id", "INTEGER", &id, "PRIMARY KEY"});
        fields.push_back({"firstname", "VARCHAR(50)", &firstname, ""});
        fields.push_back({"lastname", "VARCHAR(20)", &lastname, ""});
        fields.push_back({"email", "VARCHAR(100)", &email, ""});
        fields.push_back({"username", "VARCHAR(100)", &username, "UNIQUE"});
        fields.push_back({"password", "VARCHAR(255)", &password});
    }

    User() : SQLiteORM("users") {
        registerFields();
    }

    // Proper copy constructor
    User(const User& other) : SQLiteORM(other),
        id(other.id),
        firstname(other.firstname),
        lastname(other.lastname),
        email(other.email),
        username(other.username),
        password(other.password) {
        registerFields();
    }

    // Parameterized constructor
    User(int id, const std::string& firstname, const std::string& lastname, const std::string& email, const std::string& username, const std::string& password)
        : SQLiteORM("users"),
        id(id),
        firstname(firstname),
        lastname(lastname),
        email(email),
        username(username),
        password(password) 
    {
        registerFields();
    }

    // Virtual destructor
    virtual ~User() = default;

    ORMModel* clone() const override {
        return new User(*this);
    }


    /**
     * User Authentication methods
     */
    // Returns the user id after successfuly created the user, or returns -1
    static int create_user(
        const std::string &firstname, 
        const std::string &lastname, 
        const std::string &email, 
        const std::string &username, 
        const std::string &password
    ){
        
    }

};

#endif
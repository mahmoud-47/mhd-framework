#ifndef __USER_MODEL_H__
#define __USER_MODEL_H__

/**
 * Dont modify this class, you can inherit it for your custom users table or reference it
 * Or if you are ready to adapt the authentication methods with your new attributes you can go ahead and modify it
 */

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp" 
#include "utils/hash_password.hpp" 
#include "../utils/session/session.hpp"
#include "../utils/request/Request.hpp"

class User : public SQLiteORM {
public:
    int id;
    std::string firstname;
    std::string lastname;
    std::string email;
    std::string username;
    std::string password;
    MhdDateTime created_at;
    MhdDateTime updated_at;


    void registerFields(){
        fields.clear();
        fields.push_back({"id", "INTEGER", &id, "PRIMARY KEY"});
        fields.push_back({"firstname", "TEXT", &firstname, ""});
        fields.push_back({"lastname", "TEXT", &lastname, ""});
        fields.push_back({"email", "TEXT", &email, ""});
        fields.push_back({"username", "TEXT", &username, "UNIQUE"});
        fields.push_back({"password", "TEXT", &password});
        fields.push_back({"created_at", "DATETIME", &created_at});
        fields.push_back({"updated_at", "DATETIME", &updated_at});
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
        password(other.password),
        created_at(other.created_at),
        updated_at(other.updated_at)
    {
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

    // Returns the user pointer after successfuly created the user, or returns -1
    static User* create_user(
        const std::string &firstname, 
        const std::string &lastname, 
        const std::string &email, 
        const std::string &username, 
        const std::string &password
    ){
        std::string hashed_password = hash_password(password);

        User user(0, firstname, lastname, email, username, hashed_password);
        // capture the time right now
        MhdDateTime now;
        // set the timestamps
        user.created_at = now;
        user.updated_at = now;

        // this can raise an exception, it should be caught while creating the user
        user.save();
        
        // return the only user found
        return static_cast<User*>(user.find_by("username", username)[0]);
    }

    // Returns a pointer to the user if the authentication worked or null otherwise
    static User* authenticate(Request &request, const std::string &username, const std::string &password){
        User userQuery;
        auto users = userQuery.find_by("username", username);

        // if we don't have exactly one user with this username, return NULL
        if(users.size() != 1)
            return nullptr;

        User* user = static_cast<User*>(users[0]);

        // if the hashed password is not the same as the stored password
        if(user->password != hash_password(password))
            return nullptr;
        // add it to the session
        Session session(request);
        session.set_value("user_id", std::to_string(user->id));
        session.set_value("username", user->username);
        return user;
    }

    // Logout method
    static void logout(Request &request){
        Session session(request);
        session.remove_key("user_id");
        session.remove_key("username");
    }

    // get Authenticated User pointer or null if not authenticated
    static User* getAuthenticatedUser(Request &request){
        Session session(request);
        if(session.get_value("user_id") != ""){
            // check if user with user_id exists and return its pointer
            User userQuery;
            User* user = static_cast<User*>(userQuery.find_by_id(session.get_value("user_id")));
            return user; // returns null if user is null
        }
        return nullptr;
    }

};

#endif
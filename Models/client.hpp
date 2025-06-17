#ifndef __CLIENT_MODEL_H__
#define __CLIENT_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"
#include "../utils/datetime/mhd-date-time.hpp"  

class Client : public SQLiteORM {
public:
    int id;
    std::string name;
    std::string email;
    std::string phone;
    std::string company;
    std::string notes;
    MhdDateTime created_at;


    void registerFields(){
        fields.clear();
        fields.push_back({"id", "INTEGER", &id, "PRIMARY KEY "});
        fields.push_back({"name", "TEXT", &name, ""});
        fields.push_back({"email", "TEXT", &email, "UNIQUE"});
        fields.push_back({"phone", "TEXT", &phone, ""});
        fields.push_back({"company", "TEXT", &company, ""});
        fields.push_back({"notes", "TEXT", &notes, ""});
        fields.push_back({"created_at", "DATETIME", &created_at, ""});
    }

    Client() : SQLiteORM("clients") {
        registerFields();
    }

    // Proper copy constructor
    Client(const Client& other) : SQLiteORM(other), 
                            id(other.id),
                            name(other.name),
                            email(other.email),
                            phone(other.phone),
                            company(other.company),
                            notes(other.notes),
                            created_at(other.created_at) {
        registerFields();  // This updates the pointers to our new members
    }

    // Parameterized constructor
    Client(const int id, const std::string& name, const std::string& email, const std::string& phone, const std::string& company, const std::string& notes, const MhdDateTime& created_at) : 
        SQLiteORM("clients"), id(id), name(name), email(email), phone(phone), company(company), notes(notes), created_at(created_at) {
        registerFields();
    }

    // Virtual destructor
    virtual ~Client() = default;

    ORMModel* clone() const override {
        return new Client(*this);
    }
};

#endif
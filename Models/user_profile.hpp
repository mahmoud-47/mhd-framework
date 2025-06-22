#ifndef __USER_PROFILE_MODEL_H__
#define __USER_PROFILE_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"

class UserProfileTable : public SQLiteORM {
public:
    int id;
    int user_id;               // foreign key to users(id)
    int age;
    std::string role = "user"; // default value

    void registerFields() {
        fields.clear();

        fields.push_back({"id", "INTEGER", &id, "PRIMARY KEY"});
        fields.push_back({"user_id", "INTEGER", &user_id, "REFERENCES users(id) ON DELETE CASCADE"});
        fields.push_back({"age", "INTEGER", &age, ""});
        fields.push_back({"role", "TEXT", &role, ""});
    }

    UserProfileTable() : SQLiteORM("user_profiles") {
        registerFields();
    }

    UserProfileTable(int id, int user_id, int age, const std::string& role = "user")
        : SQLiteORM("user_profiles"), id(id), user_id(user_id), age(age), role(role) {
        registerFields();
    }

    UserProfileTable(const UserProfileTable& other)
        : SQLiteORM(other), id(other.id), user_id(other.user_id), age(other.age), role(other.role) {
        registerFields();
    }

    virtual ~UserProfileTable() = default;

    ORMModel* clone() const override {
        return new UserProfileTable(*this);
    }
};

#endif

#ifndef __USERPROFILE_MODEL_H__
#define __USERPROFILE_MODEL_H__

#include "../../utils/ORM/SQLiteORM/sqliteorm.hpp"

class UserProfile : public SQLiteORM {
public:
    int user_id;
    std::string token;
    std::string role;
    int is_active;

    void registerFields() {
        fields.clear();
        fields.push_back({"user_id", "INTEGER", &user_id, "REFERENCES users(id) ON DELETE CASCADE"});
        fields.push_back({"token", "TEXT", &token, "UNIQUE"});
        fields.push_back({"role", "TEXT", &role, ""});
        fields.push_back({"is_active", "INTEGER", &is_active, ""});
    }

    UserProfile()
        : SQLiteORM("userprofile") {
        registerFields();
    }

    UserProfile(const UserProfile& other)
        : SQLiteORM(other),
        user_id(other.user_id),
        token(other.token),
        role(other.role),
        is_active(other.is_active)
    {
        registerFields();
    }

    UserProfile(const int& user_id, const std::string& token, const std::string& role, const int& is_active)
        : SQLiteORM("userprofile"),
        user_id(user_id),
        token(token),
        role(role),
        is_active(is_active)
    {
        registerFields();
    }

    virtual ~UserProfile() = default;

    ORMModel* clone() const override {
        return new UserProfile(*this);
    }
};

#endif

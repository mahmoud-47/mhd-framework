#ifndef __MHD_SQLITE_ORM_H
#define __MHD_SQLITE_ORM_H

#include <sqlite3.h>
#include <iostream>
#include <sstream>
#include <cstring>
#include <algorithm>
#include "../orm.hpp"
#include "../../../settings.hpp"
#include "../../Exception/exception.hpp"

class SQLiteORM : public ORMModel {
protected:
    sqlite3* db;

public:
    SQLiteORM(const std::string& table)
        : ORMModel(table) {
            if (sqlite3_open(DATABASE_PATH.c_str(), &db) != SQLITE_OK) {
            std::cerr << "Failed to open SQLite DB at " << DATABASE_PATH << ": " 
                      << sqlite3_errmsg(db) << std::endl;
        }
    }

    ORMModel* clone() const {
        return new SQLiteORM(*this);
    }

    // SQLiteORM() : ORMModel("default_table_name") {
    //     db = nullptr;
    // }

    
    // Create Table
    void createTable() override;

    // Drop table
    void dropTable() override;

    // Backend execute
    bool execute(const std::string& sql) override;

    // Backend Query
    bool query(const std::string& sql, int (*callback)(void*, int, char**, char**), void* data) override;
    
    // Save object to the DB
    bool save() override;

    // Update Object, takes the id of the record
    bool update(const std::string& id) override;

    // remove by id
    bool remove(const std::string& id) override;

    // find by id
    ORMModel* find_by_id(const std::string& id) override;

    // find by column and value, the first parameter is the column name, the second one is the value
    std::vector<ORMModel*> find_by(const std::string& column_name, const std::string& value);

    std::vector<ORMModel*> find_all() override;
};

#endif

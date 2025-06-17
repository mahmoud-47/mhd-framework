#ifndef __MHD_ORM__
#define __MHD_ORM__

#include <iostream>
#include <vector>
#include <string>
#include <sqlite3.h>
#include <memory>
#include "../datetime/mhd-date-time.hpp"

// Field representation
struct Field {
    std::string name;
    std::string type;
    void* value_ptr;
    std::string constraints;

    Field(const std::string& n, const std::string& t, void* ptr, const std::string& c = "")
        : name(n), type(t), value_ptr(ptr), constraints(c) {}

    std::string to_sql_value() const {
        if (type == "TEXT") {
            return "'" + *(static_cast<std::string*>(value_ptr)) + "'";
        } else if (type == "INTEGER" || type == "INT") {
            return std::to_string(*(static_cast<int*>(value_ptr)));
        }else if(type == "REAL"){
            return std::to_string(*(static_cast<float*>(value_ptr)));
        }else if(type == "DATETIME"){
            return "'" + (*(static_cast<MhdDateTime*>(value_ptr))).to_sqlite_string() + "'";
        }
        // Add more types After
        return "NULL";
    }
};

/**
 * Custom ORM Class
 */
class ORMModel {
public:
    /**
     * Table Name
     */
    std::string table_name;
    /**
     * Table fields
     */
    std::vector<Field> fields;

    ORMModel(const std::string& name) : table_name(name) {}

    virtual ORMModel* clone() const = 0;
    
    // // Add the fields and link them to the class
    // virtual void registerFields() = 0;
    // Create table
    virtual void createTable() = 0;
    // Drop table
    virtual void dropTable() = 0;
    // Find by ID returns an instance of the class
    virtual ORMModel* find_by_id(const std::string& id) = 0;
    // get all the instances of a class
    virtual std::vector<ORMModel*> find_all() = 0;

    // save to the database
    virtual bool save() = 0;
    // Update object
    virtual bool update(const std::string& id) = 0;
    // Delete an object from the DB
    virtual bool remove(const std::string& id) = 0;

    // Execute Backend
    virtual bool execute(const std::string& sql) = 0;
    // Query Backend
    virtual bool query(const std::string& sql, int (*callback)(void*, int, char**, char**), void* data) = 0;

    // Disconnect
    virtual ~ORMModel() = default;
};


#endif
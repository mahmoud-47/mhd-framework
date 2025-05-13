#include "sqliteorm.hpp"

void SQLiteORM::createTable() {
    std::string sql = "CREATE TABLE IF NOT EXISTS " + table_name + " (";
    for (size_t i = 0; i < fields.size(); ++i) {
        sql += fields[i].name + " " + fields[i].type;
        if (!fields[i].constraints.empty()) {
            sql += " " + fields[i].constraints;
        }
        if (i < fields.size() - 1) sql += ", ";
    }
    sql += ");";
    if (showSQLQueries) {
        std::cout << "**** SQL  : " << sql << std::endl;
    }
    execute(sql);
}

void SQLiteORM::dropTable() {
    std::string sql = "DROP TABLE IF EXISTS " + table_name + ";";
    if (showSQLQueries) {
        std::cout << "**** SQL  : " << sql << std::endl;
    }
    execute(sql);
}

bool SQLiteORM::execute(const std::string& sql) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
        std::cerr << "SQLite Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        SQLException e(errMsg);
        throw e;
        return false;
    }
    return true;
}

bool SQLiteORM::query(const std::string& sql, int (*callback)(void*, int, char**, char**), void* data) {
    char* errMsg = nullptr;
    if (sqlite3_exec(db, sql.c_str(), callback, data, &errMsg) != SQLITE_OK) {
        std::cerr << "SQLite Query Error: " << errMsg << std::endl;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool SQLiteORM::save() {
    std::string sql = "INSERT INTO " + table_name + " (";
    for (size_t i = 0; i < fields.size(); ++i) {
        sql += fields[i].name;
        if (i < fields.size() - 1) sql += ", ";
    }
    sql += ") VALUES (";
    for (size_t i = 0; i < fields.size(); ++i) {
        sql += fields[i].to_sql_value();
        if (i < fields.size() - 1) sql += ", ";
    }
    sql += ");";

    if (showSQLQueries) {
        std::cout << "**** SQL  : " << sql << std::endl;
    }

    return execute(sql);
}

bool SQLiteORM::update(const std::string& id) {
    std::string sql = "UPDATE " + table_name + " SET ";
    for (size_t i = 1; i < fields.size(); ++i) {
        sql += fields[i].name + " = " + fields[i].to_sql_value();
        if (i < fields.size() - 1) sql += ", ";
    }
    sql += " WHERE " + fields[0].name + " = '" + id + "';";
    if (showSQLQueries) {
        std::cout << "**** SQL  : " << sql << std::endl;
    }
    return execute(sql);
}

bool SQLiteORM::remove(const std::string& id) {
    std::string sql = "DELETE FROM " + table_name + " WHERE " + fields[0].name + " = '" + id + "';";
    if (showSQLQueries) {
        std::cout << "**** SQL  : " << sql << std::endl;
    }
    return execute(sql);
}

ORMModel* SQLiteORM::find_by_id(const std::string& id) {
    std::string sql = "SELECT * FROM " + table_name + " WHERE " + fields[0].name + " = '" + id + "';";
    if (showSQLQueries) {
        std::cout << "**** SQL  : " << sql << std::endl;
    }
    ORMModel* result = this;
    auto callback = [](void* data, int argc, char** argv, char** colName) -> int {
        auto* model = static_cast<ORMModel*>(data);
        for (int i = 0; i < argc; ++i) {
            std::string val = argv[i] ? argv[i] : "";
            if (model->fields[i].type == "TEXT") {
                *static_cast<std::string*>(model->fields[i].value_ptr) = val;
            } else if (model->fields[i].type == "INTEGER") {
                *static_cast<int*>(model->fields[i].value_ptr) = std::stoi(val);
            }
        }
        return 0;
    };

    if (query(sql, callback, result)) return result;
    return nullptr;
}

std::vector<ORMModel*> SQLiteORM::find_all() {
    std::vector<ORMModel*> results;

    auto callback = [](void* data, int argc, char** argv, char** colName) -> int {
        auto* pair = static_cast<std::pair<SQLiteORM*, std::vector<ORMModel*>>*>(data);
        SQLiteORM* proto = pair->first;
        SQLiteORM* model = new SQLiteORM(*proto);
        for (int i = 0; i < argc; ++i) {
            std::string val = argv[i] ? argv[i] : "";
            if (model->fields[i].type == "TEXT") {
                *static_cast<std::string*>(model->fields[i].value_ptr) = val;
            } else if (model->fields[i].type == "INTEGER") {
                *static_cast<int*>(model->fields[i].value_ptr) = std::stoi(val);
            }
        }
        pair->second.push_back(model);
        return 0;
    };

    std::string sql = "SELECT * FROM " + table_name + ";";
    if (showSQLQueries) {
        std::cout << "**** SQL  : " << sql << std::endl;
    }
    std::pair<SQLiteORM*, std::vector<ORMModel*>> context = {this, results};
    query(sql, callback, &context);
    return context.second;
}
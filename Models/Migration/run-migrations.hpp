#ifndef __RUN_MIGRATIONS_HPP__
#define __RUN_MIGRATIONS_HPP__

#include <map>
#include <string>
#include "../utils/ORM/SQLiteORM/sqliteorm.hpp"
#include "../utils/ORM/dependency_graph/dependency_graph.hpp"

inline void run_migrations(ORMModel* models[], size_t count) {
    std::map<std::string, ORMModel*> tableMap;
    TableDependencyGraph depGraph;

    for (size_t i = 0; i < count; ++i) {
        ORMModel* model = models[i];
        std::string tableName = model->table_name;
        depGraph.addTable(tableName);
        tableMap[tableName] = model;

        for (const auto& field : model->fields) {
            std::string refTable = extractReferencedTable(field.constraints);
            if (!refTable.empty()) {
                depGraph.addDependency(refTable, tableName);
            }
        }
    }

    if (dropRecreateTables) {
        for (const auto& name : depGraph.getSortedTables()) {
            if (tableMap.count(name))
                tableMap[name]->dropTable();
        }
    }

    for (const auto& name : depGraph.getSortedTables()) {
        if (tableMap.count(name))
            tableMap[name]->createTable();
    }

    for (auto& entry : tableMap)
        delete entry.second;
}

#endif

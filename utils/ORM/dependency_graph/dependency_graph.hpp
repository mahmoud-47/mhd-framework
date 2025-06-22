#ifndef __MHD_GRAPH_DEP_
#define __MHD_GRAPH_DEP_

    #include <map>
    #include <vector>
    #include <set>
    #include <stack>
    #include <string>
    #include <regex>

    std::string extractReferencedTable(const std::string& constraint) {
        std::regex pattern(R"(references\s+([a-zA-Z_][a-zA-Z0-9_]*))", std::regex::icase);
        std::smatch match;

        if (std::regex_search(constraint, match, pattern)) {
            return match[1];
        }

        return "";
    }

    class TableDependencyGraph {
        private:
            std::map<std::string, std::vector<std::string>> graph;
            std::set<std::string> allTables;

            void dfs(const std::string& node, std::set<std::string>& visited, std::stack<std::string>& result) {
                visited.insert(node);
                for (const auto& dep : graph[node]) {
                    if (visited.find(dep) == visited.end())
                        dfs(dep, visited, result);
                }
                result.push(node);
            }

        public:
            void addDependency(const std::string& from, const std::string& to) {
                graph[from].push_back(to);
                allTables.insert(from);
                allTables.insert(to);
            }

            void addTable(const std::string& table) {
                allTables.insert(table);
            }

            std::vector<std::string> getSortedTables() {
                std::set<std::string> visited;
                std::stack<std::string> result;
                for (const auto& table : allTables)
                    if (visited.find(table) == visited.end())
                        dfs(table, visited, result);

                std::vector<std::string> sorted;
                while (!result.empty()) {
                    sorted.push_back(result.top());
                    result.pop();
                }
                return sorted;
            }
        };


#endif 
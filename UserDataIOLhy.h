#ifndef USERDATAIOLHY_H
#define USERDATAIOLHY_H

#include <memory>
#include <string>
#include <unordered_map>

#include "BaseGroupLhy.h"
#include "BaseUserLhy.h"

class UserDataIOLhy {
public:
    static void loadUsers(const std::string& filename,
                          std::unordered_map<std::string, std::unique_ptr<BaseUserLhy>>& users);

    static void saveUsers(const std::string& filename,
                          const std::unordered_map<std::string, std::unique_ptr<BaseUserLhy>>& users);

    static void loadGroups(const std::string& filename,
                           const std::unordered_map<std::string, std::unique_ptr<BaseUserLhy>>& users,
                           std::unordered_map<std::string, std::unique_ptr<BaseGroupLhy>>& groups);

    static void saveGroups(const std::string& filename,
                           const std::unordered_map<std::string, std::unique_ptr<BaseGroupLhy>>& groups);
};

#endif // USERDATAIOLHY_H

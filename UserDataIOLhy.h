#ifndef USERDATAIOLHY_H
#define USERDATAIOLHY_H

#include <vector>
#include <string>
#include "BaseUserLhy.h"
#include "BaseGroupLhy.h"
#include "QQUserLhy.h"
#include "WeChatUserLhy.h"
#include "QQGroupLhy.h"
#include "WeChatGroupLhy.h"

class UserDataIOLhy {
public:
    // 从文件加载用户数据
    static std::vector<BaseUserLhy*> loadUsers(const std::string& filename);

    // 保存用户数据到文件
    static void saveUsers(const std::string& filename, const std::vector<BaseUserLhy*>& users);

    // 从文件加载群数据
    static std::vector<BaseGroupLhy*> loadGroups(const std::string& filename,
        const std::vector<BaseUserLhy*>& allUsers);

    // 保存群数据到文件
    static void saveGroups(const std::string& filename, const std::vector<BaseGroupLhy*>& groups);
};

#endif // USERDATAIOLHY_H

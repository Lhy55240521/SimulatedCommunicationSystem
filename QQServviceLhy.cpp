#include "QQServiceLhy.h"
#include <iostream>

// 其他现有方法保持不变...

// 新增：创建QQ群实现
QQGroupLhy* QQServiceLhy::createGroup(const std::string& groupId, const std::string& groupName) {
    // 检查群ID是否已存在
    for (const auto& group : joinedGroups) {
        if (group->getGroupId() == groupId) {
            std::cout << "创建失败：群ID已存在！" << std::endl;
            return nullptr;
        }
    }

    // 创建新群，当前用户为群主
    QQGroupLhy* newGroup = new QQGroupLhy(groupId, groupName, user);

    // 将创建者加入群聊
    newGroup->addMember(user);
    joinedGroups.push_back(newGroup);

    std::cout << "QQ群创建成功！群ID：" << groupId << "，群名称：" << groupName << std::endl;
    return newGroup;
}

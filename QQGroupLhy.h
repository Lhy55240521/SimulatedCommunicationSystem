#ifndef QQ_GROUP_LHY_H
#define QQ_GROUP_LHY_H

#include "BaseGroupLhy.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

// QQ群子类（支持临时讨论组、申请入群、管理员制度）
class QQGroupLhy : public BaseGroupLhy {
private:
    std::vector<QQGroupLhy*> tempSubGroups;  // 临时讨论组列表（子群）
    std::vector<std::string> adminIds;       // 管理员ID列表（除群主外）

public:
    // 构造函数
    QQGroupLhy(std::string id, std::string name, BaseUserLhy* owner)
        : BaseGroupLhy(id, name, owner) {
    }

    ~QQGroupLhy() override {
        // 析构时释放临时讨论组内存
        for (auto subGroup : tempSubGroups) {
            delete subGroup;
        }
    }

    // 实现群管理接口：QQ群支持申请入群（简化：无需审核）
    bool addMember(BaseUserLhy* user) override {
        if (user == nullptr) return false;
        std::string userId = user->getId();

        if (isMember(userId)) {
            std::cout << "[QQ群] " << user->getNickname() << "（" << userId << "）已在群中" << std::endl;
            return false;
        }

        members[userId] = user;
        user->addGroupId(id);
        std::cout << "[QQ群] " << user->getNickname() << "（" << userId << "）申请入群成功，当前群成员数：" << getMemberCount() << std::endl;
        return true;
    }

    // 实现群管理接口：移除群成员（群主/管理员可操作）
    bool deleteMember(std::string userId) override {
        if (!isMember(userId)) {
            std::cout << "[QQ群] 用户" << userId << "不在群中" << std::endl;
            return false;
        }

        // 检查操作权限（群主或管理员）
        std::string operatorId = owner->getId();
        bool isAdmin = (std::find(adminIds.begin(), adminIds.end(), operatorId) != adminIds.end());
        if (operatorId != userId && !isAdmin) {
            std::cout << "[QQ群] 仅群主/管理员可移除成员" << std::endl;
            return false;
        }

        // 移除成员
        BaseUserLhy* user = members[userId];
        members.erase(userId);
        user->removeGroupId(id);
        std::cout << "[QQ群] " << user->getNickname() << "（" << userId << "）已被移除，当前群成员数：" << getMemberCount() << std::endl;
        return true;
    }

    // 实现群管理接口：查找成员（修复结构化绑定，兼容C++14）
    std::vector<BaseUserLhy*> searchMember(std::string keyword) override {
        std::vector<BaseUserLhy*> result;
        // 用迭代器遍历map，替代结构化绑定（避免E0020错误）
        for (auto it = members.begin(); it != members.end(); ++it) {
            std::string userId = it->first;    // 获取键（用户ID）
            BaseUserLhy* user = it->second;    // 获取值（用户指针）
            // 模糊匹配ID或昵称
            if (userId.find(keyword) != std::string::npos || user->getNickname().find(keyword) != std::string::npos) {
                result.push_back(user);
            }
        }

        if (result.empty()) {
            std::cout << "[QQ群] 未找到包含关键词\"" << keyword << "\"的成员" << std::endl;
        }
        else {
            std::cout << "[QQ群] 找到" << result.size() << "个包含关键词\"" << keyword << "\"的成员：" << std::endl;
            for (auto user : result) {
                std::cout << "  - " << user->getNickname() << "（" << user->getId() << "）" << std::endl;
            }
        }
        return result;
    }

    // 实现差异化接口：QQ群允许临时讨论组
    bool isTempGroupAllowed() const override {
        return true;
    }

    // QQ群特有：创建临时讨论组（核心功能，属于群类职责）
    QQGroupLhy* createTempSubGroup(std::string subGroupName, BaseUserLhy* subOwner) {
        std::string subGroupId = id + "_temp_" + std::to_string(tempSubGroups.size() + 1);
        QQGroupLhy* subGroup = new QQGroupLhy(subGroupId, subGroupName, subOwner);
        tempSubGroups.push_back(subGroup);
        std::cout << "[QQ群] 临时讨论组\"" << subGroupName << "\"（ID：" << subGroupId << "）创建成功" << std::endl;
        return subGroup;
    }

    // QQ群特有：解散临时讨论组
    bool dissolveTempSubGroup(std::string subGroupId) {
        for (auto it = tempSubGroups.begin(); it != tempSubGroups.end(); ++it) {
            if ((*it)->getGroupId() == subGroupId) {
                delete* it;
                tempSubGroups.erase(it);
                std::cout << "[QQ群] 临时讨论组（ID：" << subGroupId << "）解散成功" << std::endl;
                return true;
            }
        }
        std::cout << "[QQ群] 未找到临时讨论组（ID：" << subGroupId << "）" << std::endl;
        return false;
    }

    // QQ群特有：管理员管理
    bool addAdmin(std::string userId) {
        if (!isMember(userId) || userId == owner->getId()) {
            std::cout << "[QQ群] 添加管理员失败：用户不在群中或为群主" << std::endl;
            return false;
        }
        if (std::find(adminIds.begin(), adminIds.end(), userId) != adminIds.end()) {
            std::cout << "[QQ群] 用户" << userId << "已是管理员" << std::endl;
            return false;
        }
        adminIds.push_back(userId);
        std::cout << "[QQ群] 用户" << userId << "已添加为管理员" << std::endl;
        return true;
    }

    bool removeAdmin(std::string userId) {
        auto it = std::find(adminIds.begin(), adminIds.end(), userId);
        if (it == adminIds.end()) {
            std::cout << "[QQ群] 用户" << userId << "不是管理员" << std::endl;
            return false;
        }
        adminIds.erase(it);
        std::cout << "[QQ群] 用户" << userId << "已移除管理员身份" << std::endl;
        return true;
    }
};

#endif // QQ_GROUP_LHY_H
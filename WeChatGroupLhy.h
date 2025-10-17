#ifndef WECHAT_GROUP_LHY_H
#define WECHAT_GROUP_LHY_H

#include "BaseGroupLhy.h"
#include <vector>
#include <string>

// 微信群子类（禁止临时讨论组、仅群主推荐入群、无管理员）
class WeChatGroupLhy : public BaseGroupLhy {
private:
    std::string announcement;  // 微信群公告（特有功能）
    const int maxMemberCount = 500;  // 微信群成员上限（默认500人）

public:
    // 构造函数
    WeChatGroupLhy(std::string id, std::string name, BaseUserLhy* owner)
        : BaseGroupLhy(id, name, owner), announcement("暂无公告") {
    }

    ~WeChatGroupLhy() override = default;

    // 实现群管理接口：微信群仅群主可推荐入群
    bool addMember(BaseUserLhy* user) override {
        if (user == nullptr) return false;
        std::string userId = user->getId();

        // 检查成员上限
        if (getMemberCount() >= maxMemberCount) {
            std::cout << "[微信群] 成员已满（上限" << maxMemberCount << "人），无法添加新成员" << std::endl;
            return false;
        }

        if (isMember(userId)) {
            std::cout << "[微信群] " << user->getNickname() << "（" << userId << "）已在群中" << std::endl;
            return false;
        }

        // 仅群主可添加成员（推荐入群）
        if (userId != owner->getId()) {
            std::cout << "[微信群] 仅群主可推荐成员入群，" << user->getNickname() << "入群失败" << std::endl;
            return false;
        }

        members[userId] = user;
        user->addGroupId(id);
        std::cout << "[微信群] 群主推荐" << user->getNickname() << "（" << userId << "）入群成功，当前群成员数：" << getMemberCount() << std::endl;
        return true;
    }

    // 实现群管理接口：移除群成员（仅群主可操作）
    bool deleteMember(std::string userId) override {
        if (!isMember(userId)) {
            std::cout << "[微信群] 用户" << userId << "不在群中" << std::endl;
            return false;
        }

        // 仅群主可移除成员
        if (owner->getId() != userId) {
            std::cout << "[微信群] 仅群主可移除成员" << std::endl;
            return false;
        }

        // 移除成员
        BaseUserLhy* user = members[userId];
        members.erase(userId);
        user->removeGroupId(id);
        std::cout << "[微信群] " << user->getNickname() << "（" << userId << "）已被移除，当前群成员数：" << getMemberCount() << std::endl;
        return true;
    }

    // 实现群管理接口：查找成员（兼容C++14，用迭代器遍历）
    std::vector<BaseUserLhy*> searchMember(std::string keyword) override {
        std::vector<BaseUserLhy*> result;
        for (auto it = members.begin(); it != members.end(); ++it) {
            std::string userId = it->first;
            BaseUserLhy* user = it->second;
            if (userId.find(keyword) != std::string::npos || user->getNickname().find(keyword) != std::string::npos) {
                result.push_back(user);
            }
        }

        if (result.empty()) {
            std::cout << "[微信群] 未找到包含关键词\"" << keyword << "\"的成员" << std::endl;
        }
        else {
            std::cout << "[微信群] 找到" << result.size() << "个包含关键词\"" << keyword << "\"的成员：" << std::endl;
            for (auto user : result) {
                std::cout << "  - " << user->getNickname() << "（" << user->getId() << "）" << std::endl;
            }
        }
        return result;
    }

    // 实现差异化接口：微信群禁止临时讨论组
    bool isTempGroupAllowed() const override {
        return false;
    }

    // 微信群特有：公告管理
    void setAnnouncement(std::string content) {
        announcement = content;
        std::cout << "[微信群] 群公告已更新为：" << content << std::endl;
    }

    std::string getAnnouncement() const {
        return announcement;
    }

    // 微信群特有：获取成员上限
    int getMaxMemberCount() const {
        return maxMemberCount;
    }
};

#endif // WECHAT_GROUP_LHY_H
#ifndef WECHAT_SERVICE_LHY_H
#define WECHAT_SERVICE_LHY_H

#include "BaseServiceLhy.h"
#include "WeChatGroupLhy.h"
#include "WeChatUserLhy.h"
#include "QQUserLhy.h"
#include <vector>
#include <algorithm>
#include <iostream>

// 微信服务子类（支持QQ绑定、微信群管理）
class WeChatServiceLhy : public BaseServiceLhy {
private:
    std::vector<WeChatGroupLhy*> joinedGroups;  // 用户加入的微信群列表
    std::string bindQQId;                       // 绑定的QQID
    bool isBoundWithQQ;                         // 是否绑定QQ

public:
    // 构造函数：从用户对象获取初始绑定状态
    WeChatServiceLhy(BaseUserLhy* user) : BaseServiceLhy(user), isBoundWithQQ(false) {
        WeChatUserLhy* wxUser = dynamic_cast<WeChatUserLhy*>(user);
        if (wxUser != nullptr) {
            bindQQId = wxUser->getBindQQId();
            isBoundWithQQ = !bindQQId.empty();
            if (isBoundWithQQ) {
                std::cout << "[微信服务] 已绑定QQ：" << bindQQId << std::endl;
            }
        }
    }

    ~WeChatServiceLhy() override {
        // 释放微信群列表
        for (auto group : joinedGroups) {
            delete group;
        }
    }

    // 实现服务类型接口
    std::string getServiceType() const override {
        return "微信服务";
    }

    // 实现登录接口：微信登录（验证码固定为"6666"）
    bool login(std::string verifyInfo) override {
        if (getIsLoggedIn()) {
            std::cout << "[微信服务] " << user->getNickname() << "已登录" << std::endl;
            return true;
        }
        if (verifyInfo != "6666") {
            std::cout << "[微信服务] 登录失败：验证码错误（正确验证码：6666）" << std::endl;
            return false;
        }
        setIsLoggedIn(true);
        std::cout << "[微信服务] " << user->getNickname() << "（" << user->getId() << "）登录成功" << std::endl;
        return true;
    }

    // 微信服务特有：QQ绑定管理
    bool bindWithQQ(std::string qqId) {
        if (isBoundWithQQ) {
            std::cout << "[微信服务] 已绑定QQ：" << bindQQId << "，无需重复绑定" << std::endl;
            return false;
        }
        bindQQId = qqId;
        isBoundWithQQ = true;
        // 更新用户对象的绑定信息
        WeChatUserLhy* wxUser = dynamic_cast<WeChatUserLhy*>(user);
        if (wxUser != nullptr) {
            wxUser->setBindQQId(qqId);
        }
        std::cout << "[微信服务] 成功绑定QQ：" << qqId << std::endl;
        return true;
    }

    bool unbindQQ() {
        if (!isBoundWithQQ) {
            std::cout << "[微信服务] 未绑定QQ，无需解绑" << std::endl;
            return false;
        }
        bindQQId.clear();
        isBoundWithQQ = false;
        WeChatUserLhy* wxUser = dynamic_cast<WeChatUserLhy*>(user);
        if (wxUser != nullptr) {
            wxUser->setBindQQId("");
        }
        std::cout << "[微信服务] 成功解绑QQ" << std::endl;
        return true;
    }

    // 微信服务特有：加入微信群（需群主推荐）
    bool joinGroup(WeChatGroupLhy* group) {
        if (!getIsLoggedIn()) {
            std::cout << "[微信服务] 请先登录再加入群" << std::endl;
            return false;
        }
        // 微信群需群主操作添加，此处传入群主指针
        if (group->addMember(group->getGroupOwner())) {
            joinedGroups.push_back(group);
            return true;
        }
        return false;
    }

    // 微信服务特有：退出微信群
    bool quitGroup(std::string groupId) {
        for (auto it = joinedGroups.begin(); it != joinedGroups.end(); ++it) {
            if ((*it)->getGroupId() == groupId) {
                (*it)->deleteMember(user->getId());
                joinedGroups.erase(it);
                return true;
            }
        }
        std::cout << "[微信服务] 未加入群（ID：" << groupId << "）" << std::endl;
        return false;
    }

    // 微信服务特有：从绑定QQ获取推荐好友
    std::vector<std::string> getQQRecommendedFriends(std::vector<BaseUserLhy*> allUsers) {
        if (!isBoundWithQQ) {
            std::cout << "[微信服务] 未绑定QQ，无法获取推荐好友" << std::endl;
            return {};
        }
        // 查找绑定的QQ用户，获取其好友列表
        std::vector<std::string> recommendedFriends;
        for (auto u : allUsers) {
            QQUserLhy* qqUser = dynamic_cast<QQUserLhy*>(u);
            if (qqUser != nullptr && qqUser->getId() == bindQQId) {
                recommendedFriends = qqUser->getFriendIds();
                break;
            }
        }
        // 输出推荐结果
        if (recommendedFriends.empty()) {
            std::cout << "[微信服务] 绑定的QQ（" << bindQQId << "）无好友，无推荐" << std::endl;
        }
        else {
            std::cout << "[微信服务] 从绑定QQ（" << bindQQId << "）获取推荐好友：" << std::endl;
            for (const auto& friendId : recommendedFriends) {
                std::cout << "  - " << friendId << std::endl;
            }
        }
        return recommendedFriends;
    }

    // Get接口
    std::vector<WeChatGroupLhy*> getJoinedGroups() const { return joinedGroups; }
    bool getIsBoundWithQQ() const { return isBoundWithQQ; }
    std::string getBindQQId() const { return bindQQId; }


    WeChatGroupLhy* createGroup(const std::string& groupId, const std::string& groupName);

};

#endif // WECHAT_SERVICE_LHY_H
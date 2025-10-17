#ifndef WECHAT_USER_LHY_H
#define WECHAT_USER_LHY_H

#include "BaseUserLhy.h"

// 微信用户子类（支持与QQ绑定）
class WeChatUserLhy : public BaseUserLhy {
private:
    std::string bindQQId;  // 绑定的QQID（可能为空）

public:
    // 构造函数
    WeChatUserLhy(std::string id, std::string nickname, std::string birthDate,
        int tAge, std::string location, std::string bindQQId = "")
        : BaseUserLhy(id, nickname, birthDate, tAge, location), bindQQId(bindQQId) {
    }

    ~WeChatUserLhy() override = default;

    // 绑定QQ接口
    void setBindQQId(std::string qqId) { bindQQId = qqId; }
    std::string getBindQQId() const { return bindQQId; }

    // 实现好友管理接口
    bool addFriend(std::string friendId) override {
        if (isFriend(friendId)) return false;  // 已为好友
        friendIds.push_back(friendId);
        return true;
    }

    bool deleteFriend(std::string friendId) override {
        for (auto it = friendIds.begin(); it != friendIds.end(); ++it) {
            if (*it == friendId) {
                friendIds.erase(it);
                return true;
            }
        }
        return false;  // 不是好友
    }

    bool isFriend(std::string friendId) const override {
        for (const auto& id : friendIds) {
            if (id == friendId) return true;
        }
        return false;
    }
};

#endif // WECHAT_USER_LHY_H
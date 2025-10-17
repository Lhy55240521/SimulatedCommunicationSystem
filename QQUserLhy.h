#ifndef QQ_USER_LHY_H
#define QQ_USER_LHY_H

#include "BaseUserLhy.h"

// QQ用户子类（支持与微信绑定）
class QQUserLhy : public BaseUserLhy {
private:
    std::string bindWeChatId;  // 绑定的微信ID（可能为空）

public:
    // 构造函数
    QQUserLhy(std::string id, std::string nickname, std::string birthDate,
        int tAge, std::string location, std::string bindWeChatId = "")
        : BaseUserLhy(id, nickname, birthDate, tAge, location), bindWeChatId(bindWeChatId) {
    }

    ~QQUserLhy() override = default;

    // 绑定微信接口
    void setBindWeChatId(std::string weChatId) { bindWeChatId = weChatId; }
    std::string getBindWeChatId() const { return bindWeChatId; }

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

#endif // QQ_USER_LHY_H
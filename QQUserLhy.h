#ifndef QQ_USER_LHY_H
#define QQ_USER_LHY_H

#include "BaseUserLhy.h"

// QQ user supports optional binding with a WeChat account.
class QQUserLhy : public BaseUserLhy {
private:
    std::string bindWeChatId;

public:
    QQUserLhy(const std::string& id,
              const std::string& nickname,
              const std::string& birthDate,
              const std::string& location,
              const std::string& password = "123",
              const std::string& bindWeChatId = "");

    void setBindWeChatId(const std::string& wechatId);
    const std::string& getBindWeChatId() const { return bindWeChatId; }
    bool bindWithWeChat(const std::string& wechatId);
};

#endif // QQ_USER_LHY_H
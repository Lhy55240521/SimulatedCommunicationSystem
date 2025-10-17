#ifndef WECHAT_USER_LHY_H
#define WECHAT_USER_LHY_H

#include "BaseUserLhy.h"

// WeChat user keeps optional binding with QQ.
class WeChatUserLhy : public BaseUserLhy {
private:
    std::string bindQQId;

public:
    WeChatUserLhy(const std::string& id,
                  const std::string& nickname,
                  const std::string& birthDate,
                  int tAge,
                  const std::string& location,
                  const std::string& password = "6666",
                  const std::string& bindQQId = "");

    void setBindQQId(const std::string& qqId);
    const std::string& getBindQQId() const { return bindQQId; }
};

#endif // WECHAT_USER_LHY_H
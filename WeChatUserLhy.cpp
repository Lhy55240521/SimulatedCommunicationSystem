#include "WeChatUserLhy.h"
#include <iostream>

WeChatUserLhy::WeChatUserLhy(const std::string& id,
                             const std::string& nickname,
                             const std::string& birthDate,
                             const std::string& location,
                             const std::string& password,
                             const std::string& bindQQId)
    : BaseUserLhy(id, nickname, birthDate, location, password),
      bindQQId(bindQQId) {}

void WeChatUserLhy::setBindQQId(const std::string& qqId) {
    bindQQId = qqId;
}

bool WeChatUserLhy::bindWithQQ(const std::string& qqId) {
    if (qqId.empty()) {
        std::cout << "[WeChatUser] QQ ID cannot be empty" << std::endl;
        return false;
    }
    if (!bindQQId.empty() && bindQQId != qqId) {
        std::cout << "[WeChatUser] already bound to " << bindQQId << std::endl;
        return false;
    }
    bindQQId = qqId;
    std::cout << "[WeChatUser] bound with QQ " << qqId << std::endl;
    return true;
}

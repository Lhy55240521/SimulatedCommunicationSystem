#include "QQUserLhy.h"
#include <iostream>

QQUserLhy::QQUserLhy(const std::string& id,
                     const std::string& nickname,
                     const std::string& birthDate,
                     const std::string& location,
                     const std::string& password,
                     const std::string& bindWeChatId)
    : BaseUserLhy(id, nickname, birthDate, location, password),
      bindWeChatId(bindWeChatId) {}

void QQUserLhy::setBindWeChatId(const std::string& wechatId) {
    bindWeChatId = wechatId;
}

bool QQUserLhy::bindWithWeChat(const std::string& wechatId) {
    if (wechatId.empty()) {
        std::cout << "[QQUser] WeChat ID cannot be empty" << std::endl;
        return false;
    }
    if (!bindWeChatId.empty() && bindWeChatId != wechatId) {
        std::cout << "[QQUser] already bound to " << bindWeChatId << std::endl;
        return false;
    }
    bindWeChatId = wechatId;
    std::cout << "[QQUser] bound with WeChat " << wechatId << std::endl;
    return true;
}

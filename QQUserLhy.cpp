#include "QQUserLhy.h"

QQUserLhy::QQUserLhy(const std::string& id,
                     const std::string& nickname,
                     const std::string& birthDate,
                     int tAge,
                     const std::string& location,
                     const std::string& password,
                     const std::string& bindWeChatId)
    : BaseUserLhy(id, nickname, birthDate, tAge, location, password),
      bindWeChatId(bindWeChatId) {}

void QQUserLhy::setBindWeChatId(const std::string& wechatId) {
    bindWeChatId = wechatId;
}

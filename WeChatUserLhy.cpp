#include "WeChatUserLhy.h"

WeChatUserLhy::WeChatUserLhy(const std::string& id,
                             const std::string& nickname,
                             const std::string& birthDate,
                             int tAge,
                             const std::string& location,
                             const std::string& password,
                             const std::string& bindQQId)
    : BaseUserLhy(id, nickname, birthDate, tAge, location, password),
      bindQQId(bindQQId) {}

void WeChatUserLhy::setBindQQId(const std::string& qqId) {
    bindQQId = qqId;
}

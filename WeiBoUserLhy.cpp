#include "WeiBoUserLhy.h"

// 构造函数
WeiBoUserLhy::WeiBoUserLhy(const std::string& id, const std::string& nickname,
                           const std::string& birth, const std::string& location,
                           const std::string& password, const std::string& bindQQId)
    : BaseUserLhy(id, nickname, birth, location, password), bindQQId(bindQQId) {
    // 微博用户构造函数实现
}
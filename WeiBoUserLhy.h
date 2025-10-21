#ifndef WEIBO_USER_LHY_H
#define WEIBO_USER_LHY_H

#include "BaseUserLhy.h"
#include <string>

class WeiBoUserLhy : public BaseUserLhy {
private:
    std::string bindQQId;  // 绑定的QQ ID

public:
    // 构造函数
    WeiBoUserLhy(const std::string& id, const std::string& nickname, 
                 const std::string& birth, const std::string& location,
                 const std::string& password, const std::string& bindQQId);
    
    // 设置绑定的QQ ID
    void setBindQQId(const std::string& qqId) { bindQQId = qqId; }
    
    // 获取绑定的QQ ID
    const std::string& getBindQQId() const { return bindQQId; }
    
    // 获取服务类型
    std::string getServiceType() const { return "WEIBO"; }
};

#endif // WEIBO_USER_LHY_H
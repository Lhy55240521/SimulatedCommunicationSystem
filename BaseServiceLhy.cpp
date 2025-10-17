#include "BaseServiceLhy.h"
#include "QQServiceLhy.h"
#include "WeChatServiceLhy.h"
#include <iostream>
#include <algorithm>
#include <cctype>  // 用于toupper（忽略大小写）


std::vector<BaseServiceLhy*> BaseServiceLhy::allServices;


// 工厂方法：创建指定类型的服务
BaseServiceLhy* BaseServiceLhy::createService(std::string serviceType, BaseUserLhy* user) {
    if (user == nullptr) {
        std::cout << "[服务工厂] 创建失败：用户对象为空" << std::endl;
        return nullptr;
    }

    // 忽略大小写（支持"qq"/"QQ"/"wx"/"WeChat"等输入）
    for (auto& c : serviceType) {
        c = toupper(c);
    }

    if (serviceType == "QQ") {
        return new QQServiceLhy(user);
    }
    else if (serviceType == "WECHAT" || serviceType == "WX") {
        return new WeChatServiceLhy(user);
    }
    else {
        std::cout << "[服务工厂] 创建失败：不支持的服务类型：" << serviceType << "（仅支持QQ/WeChat）" << std::endl;
        return nullptr;
    }
}

// 基类纯虚析构函数实现（从服务列表移除当前实例）
BaseServiceLhy::~BaseServiceLhy() {
    auto it = std::find(allServices.begin(), allServices.end(), this);
    if (it != allServices.end()) {
        allServices.erase(it);
    }
}
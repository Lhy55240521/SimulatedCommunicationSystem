#ifndef BASE_SERVICE_LHY_H
#define BASE_SERVICE_LHY_H

#include "BaseUserLhy.h"
#include <vector>
#include <string>
#include <iostream>


class BaseServiceLhy {
private:
    bool isLoggedIn;                      // 服务登录状态（仍为私有）

protected:
    BaseUserLhy* user;                    // 关联的用户对象
    // 仅声明静态成员，不定义
    static std::vector<BaseServiceLhy*> allServices;  // 声明
    //virtual std::string getServiceType() const = 0;

public:
    BaseServiceLhy(BaseUserLhy* user) : user(user), isLoggedIn(false) {
        allServices.push_back(this);
    }

    virtual ~BaseServiceLhy() = 0;

    // 核心接口：登录与同步

    virtual std::string getServiceType() const = 0;   // 服务类型接口

    static std::vector<BaseServiceLhy*>& getAllServices() {
        return allServices;
    }

    virtual bool login(std::string verifyInfo) = 0;
    virtual void logout() {
        isLoggedIn = false;
        // 修复：子类可访问 user，此处可正常调用 user->getNickname()
        std::cout << "[" << getServiceType() << "] " << user->getNickname() << "已退出登录" << std::endl;
    }

    void syncLogin() {
        if (!isLoggedIn) return;
        std::cout << "\n[登录同步] " << user->getNickname() << "的其他服务开始自动登录..." << std::endl;

        // 使用公有接口访问，替代直接访问 allServices
        for (auto service : getAllServices()) {
            if (service != this && !service->isLoggedIn) {
                service->isLoggedIn = true;
                std::cout << "[登录同步] " << service->getServiceType() << "已自动登录" << std::endl;
            }
        }
        std::cout << "[登录同步] 所有服务登录完成\n" << std::endl;
    }

    // 工厂方法：创建服务实例
    static BaseServiceLhy* createService(std::string serviceType, BaseUserLhy* user);

    // 公有访问接口：外部如需访问 user，通过该接口（可选，按需添加）
    BaseUserLhy* getUser() const {
        return user;
    }

    // Get/Set 接口
    bool getIsLoggedIn() const { return isLoggedIn; }
    void setIsLoggedIn(bool status) { isLoggedIn = status; }
};

// 静态成员初始化
//std::vector<BaseServiceLhy*> BaseServiceLhy::allServices;

#endif // BASE_SERVICE_LHY_H
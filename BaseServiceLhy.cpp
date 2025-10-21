#include "BaseServiceLhy.h"
#include "QQServiceLhy.h"
#include "WeChatServiceLhy.h"
#include "WeiBoServiceLhy.h"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <sstream>

std::unordered_map<std::string, std::vector<BaseServiceLhy*>> BaseServiceLhy::userServices;

BaseServiceLhy::BaseServiceLhy(BaseUserLhy* userPtr, const std::string& type)
    : user(userPtr), loggedIn(false), serviceType(type) {
    if (user) {
        userServices[user->getId()].push_back(this);
    }
}

BaseServiceLhy::~BaseServiceLhy() {
    if (!user) {
        return;
    }
    auto& list = userServices[user->getId()];
    list.erase(std::remove(list.begin(), list.end(), this), list.end());
}

void BaseServiceLhy::finishLogin() {
    if (!user) {
        return;
    }
    loggedIn = true;
    user->openService(serviceType);
    syncLoginForUser(user->getId(), this);
}

void BaseServiceLhy::logout() {
    if (!loggedIn) {
        return;
    }
    loggedIn = false;
    std::cout << "[" << serviceType << "] " << user->getNickname() << " logged out" << std::endl;
}

void BaseServiceLhy::syncLoginForUser(const std::string& userId, BaseServiceLhy* origin) {
    auto it = userServices.find(userId);
    if (it == userServices.end()) {
        return;
    }
    for (auto* service : it->second) {
        if (service != origin) {
            service->setLoggedIn(true);
            std::cout << "[AutoLogin] " << service->getServiceType() << " logged in automatically" << std::endl;
        }
    }
}

void BaseServiceLhy::registerOpenedServices(BaseUserLhy* user, const std::vector<std::string>& serviceTypes) {
    if (!user) {
        return;
    }
    for (const auto& type : serviceTypes) {
        if (!type.empty()) {
            user->openService(type);
        }
    }
}

std::unique_ptr<BaseServiceLhy> BaseServiceLhy::create(const std::string& rawType, BaseUserLhy* user) {
    if (!user) {
        return nullptr;
    }

    std::string normalized = rawType;
    std::transform(normalized.begin(), normalized.end(), normalized.begin(), [](unsigned char c) {
        return static_cast<char>(std::toupper(c));
    });

    if (normalized == "QQ") {
        return std::unique_ptr<BaseServiceLhy>(new QQServiceLhy(user));
    }
    if (normalized == "WECHAT" || normalized == "WX") {
        return std::unique_ptr<BaseServiceLhy>(new WeChatServiceLhy(user));
    }
    if (normalized == "WEIBO") {
        return std::unique_ptr<BaseServiceLhy>(new WeiBoServiceLhy(user));
    }
    std::cout << "[Factory] Unsupported service type: " << rawType << std::endl;
    return nullptr;
}
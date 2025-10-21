#include "WeChatServiceLhy.h"
#include "WeChatGroupLhy.h"

#include <iostream>

WeChatServiceLhy::WeChatServiceLhy(BaseUserLhy* user)
    : BaseServiceLhy(user, "WECHAT") {}

bool WeChatServiceLhy::login(const std::string& credential) {
    if (isLoggedIn()) {
        std::cout << "[WeChat] already logged in" << std::endl;
        return true;
    }
    if (!getUser()->verifyPassword(credential)) {
        std::cout << "[WeChat] invalid verification code" << std::endl;
        return false;
    }
    finishLogin();
    std::cout << "[WeChat] " << getUser()->getNickname() << " logged in" << std::endl;
    return true;
}

void WeChatServiceLhy::updateRecommendations(const std::vector<std::string>& friends) {
    recommendedFriends = friends;
}

WeChatGroupLhy* WeChatServiceLhy::createGroup(const std::string& groupId, const std::string& groupName) {
    if (!isLoggedIn()) {
        std::cout << "[WeChat] please login first" << std::endl;
        return nullptr;
    }
    std::cout << "[WeChat] creating group " << groupId << " (" << groupName << ")" << std::endl;
    return new WeChatGroupLhy(groupId, groupName, getUser());
}

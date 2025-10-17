#include "WeChatServiceLhy.h"

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

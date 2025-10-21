#include "WeiBoServiceLhy.h"
#include "WeiBoUserLhy.h"
#include <iostream>

// 构造函数
WeiBoServiceLhy::WeiBoServiceLhy(BaseUserLhy* user) 
    : BaseServiceLhy(user, "WEIBO") {
    // 初始化一些默认的热门话题
    trendingTopics = {"科技", "娱乐", "体育", "财经", "教育"};
}

// 重写登录函数
bool WeiBoServiceLhy::login(const std::string& credential) {
    // 验证用户凭证
    if (getUser() && getUser()->verifyPassword(credential)) {
        // 检查是否是WeiBoUserLhy类型的用户
        WeiBoUserLhy* weiBoUser = dynamic_cast<WeiBoUserLhy*>(getUser());
        if (weiBoUser) {
            std::cout << "[WeiBo] 用户 " << getUser()->getId() << " 登录成功" << std::endl;
            
            // 显示热门话题
            std::cout << "[WeiBo] 当前热门话题：" << std::endl;
            for (const auto& topic : trendingTopics) {
                std::cout << "  - " << topic << std::endl;
            }
            
            // 调用基类的finishLogin方法
            finishLogin();
            return true;
        }
    }
    return false;
}

// 更新热门话题
void WeiBoServiceLhy::updateTrendingTopics(const std::vector<std::string>& topics) {
    trendingTopics = topics;
}
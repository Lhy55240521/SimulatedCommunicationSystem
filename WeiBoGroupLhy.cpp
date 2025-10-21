#include "WeiBoGroupLhy.h"
#include "BaseUserLhy.h"
#include <iostream>
#include <algorithm>

// 构造函数
WeiBoGroupLhy::WeiBoGroupLhy(const std::string& id, const std::string& name, 
                             BaseUserLhy* owner, const std::string& topic)
    : BaseGroupLhy(id, name, "WEIBO", owner), topic(topic) {
    // 微博群组构造函数实现
}

// 重写添加成员函数
bool WeiBoGroupLhy::addMember(BaseUserLhy* user) {
    // 检查群组成员数量是否超过上限（假设微博群组最多100人）
    if (memberCount() >= 100) {
        std::cout << "[WeiBoGroup] 群组成员数量已达上限（100人）" << std::endl;
        return false;
    }
    
    // 检查用户是否已经是成员
    if (isMember(user->getId())) {
        std::cout << "[WeiBoGroup] 用户 " << user->getId() << " 已经是群组成员" << std::endl;
        return false;
    }
    
    // 检查用户是否有权限加入（这里简化为只有QQ或微博用户可以加入）
    std::string userType = user->getServiceType();
    if (userType != "QQ" && userType != "WEIBO") {
        std::cout << "[WeiBoGroup] 只有QQ用户或微博用户可以加入微博群组" << std::endl;
        return false;
    }
    
    // 添加成员
    if (addMemberRecord(user)) {
        std::cout << "[WeiBoGroup] 用户 " << user->getId() << " 成功加入群组" << std::endl;
        return true;
    }
    
    return false;
}

// 重写移除成员函数
bool WeiBoGroupLhy::removeMember(const std::string& userId, const std::string& actorId) {
    // 检查用户是否是成员
    if (!isMember(userId)) {
        std::cout << "[WeiBoGroup] 用户 " << userId << " 不是群组成员" << std::endl;
        return false;
    }
    
    // 检查是否是群主尝试移除自己
    if (userId == getOwner()->getId()) {
        std::cout << "[WeiBoGroup] 群主不能移除自己" << std::endl;
        return false;
    }
    
    // 移除成员
    if (removeMemberRecord(userId)) {
        std::cout << "[WeiBoGroup] 用户 " << userId << " 已从群组移除" << std::endl;
        return true;
    }
    
    return false;
}

// 显示群组特性摘要
void WeiBoGroupLhy::showFeatureSummary() const {
    std::cout << "[WeiBoGroup] 群组特性摘要：" << std::endl;
    std::cout << "  - 群组ID: " << id << std::endl;
    std::cout << "  - 群组名称: " << name << std::endl;
    std::cout << "  - 群组话题: " << topic << std::endl;
    std::cout << "  - 成员数量: " << members.size() << std::endl;
    std::cout << "  - 标签数量: " << hashtags.size() << std::endl;
    
    if (!hashtags.empty()) {
        std::cout << "  - 标签列表: ";
        for (size_t i = 0; i < hashtags.size(); ++i) {
            if (i > 0) std::cout << ", ";
            std::cout << "#" << hashtags[i];
        }
        std::cout << std::endl;
    }
}

// 添加标签
void WeiBoGroupLhy::addHashtag(const std::string& hashtag) {
    // 检查标签是否已存在
    if (std::find(hashtags.begin(), hashtags.end(), hashtag) == hashtags.end()) {
        hashtags.push_back(hashtag);
        std::cout << "[WeiBoGroup] 标签 #" << hashtag << " 已添加到群组 " << id << std::endl;
    } else {
        std::cout << "[WeiBoGroup] 标签 #" << hashtag << " 已存在于群组 " << id << std::endl;
    }
}

// 移除标签
void WeiBoGroupLhy::removeHashtag(const std::string& hashtag) {
    auto it = std::find(hashtags.begin(), hashtags.end(), hashtag);
    if (it != hashtags.end()) {
        hashtags.erase(it);
        std::cout << "[WeiBoGroup] 标签 #" << hashtag << " 已从群组 " << id << " 移除" << std::endl;
    } else {
        std::cout << "[WeiBoGroup] 标签 #" << hashtag << " 不存在于群组 " << id << std::endl;
    }
}
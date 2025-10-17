#include "WeChatGroupLhy.h"

#include <iostream>

WeChatGroupLhy::WeChatGroupLhy(const std::string& id,
                               const std::string& name,
                               BaseUserLhy* owner,
                               size_t maxMembers)
    : BaseGroupLhy(id, name, "WECHAT", owner),
      announcement("Welcome"),
      maxMembers(maxMembers) {}

bool WeChatGroupLhy::addMember(BaseUserLhy* user) {
    if (!user) {
        return false;
    }
    if (memberCount() >= maxMembers) {
        std::cout << "[WeChatGroup] member capacity reached" << std::endl;
        return false;
    }
    if (!addMemberRecord(user)) {
        std::cout << "[WeChatGroup] " << user->getId() << " already joined" << std::endl;
        return false;
    }
    std::cout << "[WeChatGroup] " << user->getId() << " joined" << std::endl;
    return true;
}

bool WeChatGroupLhy::removeMember(const std::string& userId, const std::string& actorId) {
    if (actorId != getOwner()->getId()) {
        std::cout << "[WeChatGroup] only owner may remove members" << std::endl;
        return false;
    }
    if (userId == getOwner()->getId()) {
        std::cout << "[WeChatGroup] owner cannot leave" << std::endl;
        return false;
    }
    if (!removeMemberRecord(userId)) {
        std::cout << "[WeChatGroup] member not found" << std::endl;
        return false;
    }
    std::cout << "[WeChatGroup] member " << userId << " removed" << std::endl;
    return true;
}

void WeChatGroupLhy::showFeatureSummary() const {
    std::cout << "[WeChatGroup] owner invites, no admins, announcement driven" << std::endl;
}

void WeChatGroupLhy::setAnnouncement(const std::string& message) {
    announcement = message;
    std::cout << "[WeChatGroup] announcement updated" << std::endl;
}

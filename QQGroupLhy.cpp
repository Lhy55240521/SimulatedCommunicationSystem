#include "QQGroupLhy.h"

#include <algorithm>
#include <iostream>

QQGroupLhy::QQGroupLhy(const std::string& id,
                       const std::string& name,
                       BaseUserLhy* owner)
    : BaseGroupLhy(id, name, "QQ", owner) {
    if (owner) {
        adminIds.insert(owner->getId());
    }
}

bool QQGroupLhy::addMember(BaseUserLhy* user) {
    if (!user) {
        return false;
    }
    if (!addMemberRecord(user)) {
        std::cout << "[QQGroup] " << user->getId() << " already joined" << std::endl;
        return false;
    }
    std::cout << "[QQGroup] " << user->getId() << " joined group " << getId() << std::endl;
    return true;
}

bool QQGroupLhy::removeMember(const std::string& userId, const std::string& actorId) {
    if (userId == getOwner()->getId()) {
        std::cout << "[QQGroup] owner cannot be removed" << std::endl;
        return false;
    }
    if (!isMember(userId)) {
        std::cout << "[QQGroup] member not found" << std::endl;
        return false;
    }
    if (actorId != getOwner()->getId() && !isAdmin(actorId)) {
        std::cout << "[QQGroup] only owner or admins may remove members" << std::endl;
        return false;
    }
    removeMemberRecord(userId);
    adminIds.erase(userId);
    std::cout << "[QQGroup] member " << userId << " removed" << std::endl;
    return true;
}

void QQGroupLhy::showFeatureSummary() const {
    std::cout << "[QQGroup] supports join requests, admins, and temporary sub groups" << std::endl;
}

bool QQGroupLhy::addAdmin(const std::string& userId) {
    if (!isMember(userId)) {
        std::cout << "[QQGroup] member not found" << std::endl;
        return false;
    }
    if (userId == getOwner()->getId()) {
        std::cout << "[QQGroup] owner already has privileges" << std::endl;
        return false;
    }
    auto inserted = adminIds.insert(userId);
    if (inserted.second) {
        std::cout << "[QQGroup] " << userId << " promoted to admin" << std::endl;
    }
    return inserted.second;
}

bool QQGroupLhy::removeAdmin(const std::string& userId) {
    auto erased = adminIds.erase(userId);
    if (erased == 0) {
        std::cout << "[QQGroup] admin not found" << std::endl;
        return false;
    }
    std::cout << "[QQGroup] admin " << userId << " revoked" << std::endl;
    return true;
}

bool QQGroupLhy::isAdmin(const std::string& userId) const {
    return adminIds.find(userId) != adminIds.end();
}

std::string QQGroupLhy::createTempSubGroup(const std::string& name, BaseUserLhy* subOwner) {
    std::string subGroupId = getId() + "_temp_" + std::to_string(tempSubGroupIds.size() + 1);
    tempSubGroupIds.push_back(subGroupId + "(" + name + ")");
    std::cout << "[QQGroup] created temp subgroup " << subGroupId << " lead by "
              << (subOwner ? subOwner->getId() : "unknown") << std::endl;
    return subGroupId;
}

bool QQGroupLhy::dissolveTempSubGroup(const std::string& subGroupId) {
    auto it = std::find_if(tempSubGroupIds.begin(), tempSubGroupIds.end(), [&](const std::string& entry) {
        return entry.rfind(subGroupId, 0) == 0;
    });
    if (it == tempSubGroupIds.end()) {
        std::cout << "[QQGroup] temp subgroup not found" << std::endl;
        return false;
    }
    tempSubGroupIds.erase(it);
    std::cout << "[QQGroup] temp subgroup " << subGroupId << " dissolved" << std::endl;
    return true;
}

void QQGroupLhy::restoreTempSubGroups(const std::vector<std::string>& entries) {
    tempSubGroupIds = entries;
}

std::vector<std::string> QQGroupLhy::getAdminIds() const {
    return {adminIds.begin(), adminIds.end()};
}

bool QQGroupLhy::requestJoin(const std::string& userId, const std::string& message) {
    if (isMember(userId)) {
        std::cout << "[QQGroup] already a member" << std::endl;
        return false;
    }
    joinRequests[userId] = message;
    std::cout << "[QQGroup] join request from " << userId << " recorded" << std::endl;
    return true;
}

bool QQGroupLhy::approveJoinRequest(const std::string& userId, const std::string& approverId, BaseUserLhy* applicant) {
    if (approverId != getOwner()->getId() && !isAdmin(approverId)) {
        std::cout << "[QQGroup] only owner or admins can approve requests" << std::endl;
        return false;
    }
    auto it = joinRequests.find(userId);
    if (it == joinRequests.end()) {
        std::cout << "[QQGroup] no join request from " << userId << std::endl;
        return false;
    }
    joinRequests.erase(it);
    if (applicant && addMember(applicant)) {
        std::cout << "[QQGroup] join request approved by " << approverId << std::endl;
        return true;
    }
    return false;
}

bool QQGroupLhy::rejectJoinRequest(const std::string& userId, const std::string& rejectorId) {
    if (rejectorId != getOwner()->getId() && !isAdmin(rejectorId)) {
        std::cout << "[QQGroup] only owner or admins can reject requests" << std::endl;
        return false;
    }
    auto erased = joinRequests.erase(userId);
    if (erased > 0) {
        std::cout << "[QQGroup] join request from " << userId << " rejected" << std::endl;
        return true;
    }
    std::cout << "[QQGroup] no join request from " << userId << std::endl;
    return false;
}

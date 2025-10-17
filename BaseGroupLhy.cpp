#include "BaseGroupLhy.h"

#include <algorithm>

BaseGroupLhy::BaseGroupLhy(const std::string& groupId,
                           const std::string& groupName,
                           const std::string& type,
                           BaseUserLhy* ownerUser)
    : id(groupId),
      name(groupName),
      serviceType(type),
      owner(ownerUser) {
    if (owner) {
        addMemberRecord(owner);
    }
}

bool BaseGroupLhy::addMemberRecord(BaseUserLhy* user) {
    if (!user) {
        return false;
    }
    if (members.count(user->getId()) > 0) {
        return false;
    }
    members[user->getId()] = user;
    user->addGroupId(id);
    return true;
}

bool BaseGroupLhy::removeMemberRecord(const std::string& userId) {
    auto it = members.find(userId);
    if (it == members.end()) {
        return false;
    }
    it->second->removeGroupId(id);
    members.erase(it);
    return true;
}

std::vector<BaseUserLhy*> BaseGroupLhy::getMemberList() const {
    std::vector<BaseUserLhy*> result;
    result.reserve(members.size());
    for (const auto& pair : members) {
        result.push_back(pair.second);
    }
    return result;
}

bool BaseGroupLhy::isMember(const std::string& userId) const {
    return members.find(userId) != members.end();
}

bool BaseGroupLhy::restoreMember(BaseUserLhy* user) {
    return addMemberRecord(user);
}

std::vector<BaseUserLhy*> BaseGroupLhy::searchMembers(const std::string& keyword) const {
    if (keyword.empty()) {
        return getMemberList();
    }
    std::vector<BaseUserLhy*> result;
    for (const auto& pair : members) {
        if (pair.first.find(keyword) != std::string::npos ||
            pair.second->getNickname().find(keyword) != std::string::npos) {
            result.push_back(pair.second);
        }
    }
    return result;
}

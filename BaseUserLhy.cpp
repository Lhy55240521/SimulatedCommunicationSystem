#include "BaseUserLhy.h"

#include <algorithm>

BaseUserLhy::BaseUserLhy(const std::string& id,
                         const std::string& nickname,
                         const std::string& birthDate,
                         int tAge,
                         const std::string& location,
                         const std::string& password)
    : id(id),
      nickname(nickname),
      birthDate(birthDate),
      tAge(tAge),
      location(location),
      password(password) {}

std::vector<std::string> BaseUserLhy::getFriendIds() const {
    return {friendIds.begin(), friendIds.end()};
}

std::vector<std::string> BaseUserLhy::getGroupIds() const {
    return {groupIds.begin(), groupIds.end()};
}

std::vector<std::string> BaseUserLhy::getOpenedServices() const {
    return {openedServices.begin(), openedServices.end()};
}

bool BaseUserLhy::verifyPassword(const std::string& input) const {
    return password == input;
}

bool BaseUserLhy::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (!verifyPassword(oldPassword)) {
        return false;
    }
    password = newPassword;
    return true;
}

bool BaseUserLhy::addFriend(const std::string& friendId) {
    if (friendId.empty() || friendId == id) {
        return false;
    }
    auto inserted = friendIds.insert(friendId);
    if (!inserted.second) {
        return false;
    }
    // Clear stale remark when re-adding with the same id.
    friendRemarks.erase(friendId);
    return true;
}

bool BaseUserLhy::removeFriend(const std::string& friendId) {
    auto erased = friendIds.erase(friendId);
    friendRemarks.erase(friendId);
    return erased > 0;
}

bool BaseUserLhy::hasFriend(const std::string& friendId) const {
    return friendIds.find(friendId) != friendIds.end();
}

void BaseUserLhy::setFriendRemark(const std::string& friendId, const std::string& remark) {
    if (!hasFriend(friendId)) {
        return;
    }
    if (remark.empty()) {
        friendRemarks.erase(friendId);
    } else {
        friendRemarks[friendId] = remark;
    }
}

void BaseUserLhy::addGroupId(const std::string& groupId) {
    if (!groupId.empty()) {
        groupIds.insert(groupId);
    }
}

bool BaseUserLhy::removeGroupId(const std::string& groupId) {
    return groupIds.erase(groupId) > 0;
}

bool BaseUserLhy::isInGroup(const std::string& groupId) const {
    return groupIds.find(groupId) != groupIds.end();
}

void BaseUserLhy::openService(const std::string& serviceType) {
    if (!serviceType.empty()) {
        openedServices.insert(serviceType);
    }
}

bool BaseUserLhy::hasOpenedService(const std::string& serviceType) const {
    return openedServices.find(serviceType) != openedServices.end();
}

#include "BaseUserLhy.h"
#include <algorithm>

BaseUserLhy::BaseUserLhy(const std::string& id, const std::string& nickname,
    const std::string& birthDate, int tAge, const std::string& location,
    const std::string& password)
    : id(id), nickname(nickname), birthDate(birthDate), tAge(tAge),
    location(location), password(password) {
}


// ��������ȡ����


bool BaseUserLhy::addFriend(std::string friendId) {
    // ����Ƿ��Ѿ��Ǻ���
    if (std::find(friendIds.begin(), friendIds.end(), friendId) != friendIds.end()) {
        return false;
    }
    friendIds.push_back(friendId);
    return true;
}

bool BaseUserLhy::deleteFriend(std::string friendId) {
    auto it = std::find(friendIds.begin(), friendIds.end(), friendId);
    if (it == friendIds.end()) {
        return false;
    }
    friendIds.erase(it);
    return true;
}

bool BaseUserLhy::isFriend(std::string friendId) const {
    return std::find(friendIds.begin(), friendIds.end(), friendId) != friendIds.end();
}

bool BaseUserLhy::verifyPassword(const std::string& inputPassword) const {
    return password == inputPassword;
}

bool BaseUserLhy::changePassword(const std::string& oldPassword, const std::string& newPassword) {
    if (verifyPassword(oldPassword)) {
        password = newPassword;
        return true;
    }
    return false;
}

void BaseUserLhy::changeNickname(const std::string& newNickname) {
    nickname = newNickname;
}

void BaseUserLhy::changeLocation(const std::string& newLocation) {
    location = newLocation;
}

void BaseUserLhy::changeBirthDate(const std::string& newBirthDate) {
    birthDate = newBirthDate;
}

#ifndef BASE_USER_LHY_H
#define BASE_USER_LHY_H

#include <map>
#include <set>
#include <string>
#include <vector>

// Base class that stores shared user attributes and neutral friend/group logic.
class BaseUserLhy {
protected:
    std::string id;
    std::string nickname;
    std::string birthDate;
    int tAge;
    std::string location;
    std::string password;
    std::set<std::string> friendIds;
    std::map<std::string, std::string> friendRemarks;
    std::set<std::string> groupIds;
    std::set<std::string> openedServices;

public:
    BaseUserLhy(const std::string& id,
                const std::string& nickname,
                const std::string& birthDate,
                int tAge,
                const std::string& location,
                const std::string& password = "123");
    virtual ~BaseUserLhy() = default;

    const std::string& getId() const { return id; }
    const std::string& getNickname() const { return nickname; }
    const std::string& getBirthDate() const { return birthDate; }
    int getTAge() const { return tAge; }
    const std::string& getLocation() const { return location; }
    const std::string& getPassword() const { return password; }
    std::vector<std::string> getFriendIds() const;
    std::vector<std::string> getGroupIds() const;
    const std::map<std::string, std::string>& getFriendRemarks() const { return friendRemarks; }
    std::vector<std::string> getOpenedServices() const;

    void setNickname(const std::string& value) { nickname = value; }
    void setLocation(const std::string& value) { location = value; }
    void setBirthDate(const std::string& value) { birthDate = value; }
    void setTAge(int value) { tAge = value; }

    bool verifyPassword(const std::string& input) const;
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);

    bool addFriend(const std::string& friendId);
    bool removeFriend(const std::string& friendId);
    bool hasFriend(const std::string& friendId) const;
    void setFriendRemark(const std::string& friendId, const std::string& remark);

    void addGroupId(const std::string& groupId);
    bool removeGroupId(const std::string& groupId);
    bool isInGroup(const std::string& groupId) const;

    void openService(const std::string& serviceType);
    bool hasOpenedService(const std::string& serviceType) const;
};

#endif // BASE_USER_LHY_H
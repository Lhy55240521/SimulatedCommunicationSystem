#ifndef BASE_USER_LHY_H
#define BASE_USER_LHY_H

#include <map>
#include <set>
#include <string>
#include <vector>
#include <chrono>
#include <ctime>

// Base class that stores shared user attributes and neutral friend/group logic.
class BaseUserLhy {
protected:
    std::string id;
    std::string nickname;
    std::string birthDate;
    std::string accountCreationTime;//t龄
    std::string location;
    std::string password;
    std::set<std::string> friendIds;
    std::map<std::string, std::string> friendRemarks;
    std::set<std::string> groupIds;
    std::set<std::string> openedServices;
    std::map<std::string, std::string> servicePasswords;  // 服务特定密码映射

public:
    BaseUserLhy(const std::string& id,
                const std::string& nickname,
                const std::string& birthDate,
                const std::string& location,
                const std::string& password = "5524");
    virtual ~BaseUserLhy() = default;

    // 获取用户ID
    const std::string& getId() const { return id; }
    // 获取用户昵称
    const std::string& getNickname() const { return nickname; }
    // 获取用户出生日期
    const std::string& getBirthDate() const { return birthDate; }
    // 获取账户创建时间
    const std::string& getAccountCreationTime() const { return accountCreationTime; }
    // 获取用户地理位置
    const std::string& getLocation() const { return location; }
    // 获取用户密码
    const std::string& getPassword() const { return password; }
    // 获取用户的好友ID列表
    std::vector<std::string> getFriendIds() const;
    // 获取用户加入的群组ID列表
    std::vector<std::string> getGroupIds() const;
    // 获取用户的好友备注信息
    const std::map<std::string, std::string>& getFriendRemarks() const { return friendRemarks; }
    // 获取用户已开通的服务列表
    std::vector<std::string> getOpenedServices() const;

    // 设置用户昵称
    void setNickname(const std::string& value) { nickname = value; }
    // 设置用户地理位置
    void setLocation(const std::string& value) { location = value; }
    // 设置用户出生日期
    void setBirthDate(const std::string& value) { birthDate = value; }
    // 设置账户创建时间
    void setAccountCreationTime(const std::string& value) { accountCreationTime = value; }

    // 验证用户输入的密码是否正确
    bool verifyPassword(const std::string& input) const;
    // 修改用户密码
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);

    // 获取服务类型（虚函数）
    virtual std::string getServiceType() const { return "BASE"; }

    // 添加好友
    bool addFriend(const std::string& friendId);
    // 删除好友
    bool removeFriend(const std::string& friendId);
    // 检查是否为好友
    bool hasFriend(const std::string& friendId) const;
    // 设置好友备注
    void setFriendRemark(const std::string& friendId, const std::string& remark);

    // 添加群组ID
    void addGroupId(const std::string& groupId);
    // 删除群组ID
    bool removeGroupId(const std::string& groupId);
    // 检查是否在某个群组中
    bool isInGroup(const std::string& groupId) const;

    // 开通服务
    void openService(const std::string& serviceType);
    // 检查是否已开通某项服务
    bool hasOpenedService(const std::string& serviceType) const;
    
    // 服务特定密码管理
    void setServicePassword(const std::string& serviceType, const std::string& password);
    const std::string& getServicePassword(const std::string& serviceType) const;
    bool verifyServicePassword(const std::string& serviceType, const std::string& input) const;
};
#endif // BASE_USER_LHY_H

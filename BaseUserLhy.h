#ifndef BASE_USER_LHY_H
#define BASE_USER_LHY_H

#include <string>
#include <vector>

// 抽象基类：所有用户的公共父类
class BaseUserLhy {

protected:
    std::string id;
    std::string nickname;
    std::string password; // 存储密码
    std::string birthDate;
    int tAge;
    std::string location;
    std::vector<std::string> friendIds;
    std::vector<std::string> groupIds;   // 群ID列表


public:
    // 构造函数
    BaseUserLhy(const std::string& id, const std::string& nickname,
        const std::string& birthDate, int tAge, const std::string& location,
        const std::string& password = "123"); // 默认密码

    // 纯虚析构函数（确保子类析构正常调用）
    virtual ~BaseUserLhy() = 0;

    // 通用Get接口
    std::string getId() const { return id; }
    std::string getNickname() const { return nickname; }
    std::string getBirthDate() const { return birthDate; }
    int getTAge() const { return tAge; }
    std::string getLocation() const { return location; }
    std::vector<std::string> getFriendIds() const { return friendIds; }
    std::vector<std::string> getGroupIds() const { return groupIds; }
    std::string getPassword() const { return password; }

    // 通用Set接口
    void setNickname(std::string newNickname) { nickname = newNickname; }
    void setLocation(std::string newLocation) { location = newLocation; }

    // 好友管理纯虚接口（子类统一实现）
    virtual bool addFriend(std::string friendId) = 0;
    virtual bool deleteFriend(std::string friendId) = 0;
    virtual bool isFriend(std::string friendId) const = 0;

    // 群管理通用接口
    void addGroupId(std::string groupId) { groupIds.push_back(groupId); }
    bool removeGroupId(std::string groupId) {
        for (auto it = groupIds.begin(); it != groupIds.end(); ++it) {
            if (*it == groupId) {
                groupIds.erase(it);
                return true;
            }
        }
        return false;
    }


    // 新增：验证密码方法
    bool verifyPassword(const std::string& inputPassword) const;

    // 新增：修改信息方法
    bool changePassword(const std::string& oldPassword, const std::string& newPassword);
    void changeNickname(const std::string& newNickname);
    void changeLocation(const std::string& newLocation);
    void changeBirthDate(const std::string& newBirthDate);

};

// 纯虚析构函数必须有实现
inline BaseUserLhy::~BaseUserLhy() = default;

#endif // BASE_USER_LHY_H
#ifndef BASE_GROUP_LHY_H
#define BASE_GROUP_LHY_H

#include "BaseUserLhy.h"

#include <map>
#include <string>
#include <vector>

class BaseGroupLhy {
protected:
    std::string id;
    std::string name;
    std::string serviceType;
    BaseUserLhy* owner;
    std::map<std::string, BaseUserLhy*> members;

    bool addMemberRecord(BaseUserLhy* user);
    bool removeMemberRecord(const std::string& userId);

public:
    BaseGroupLhy(const std::string& groupId,
                 const std::string& groupName,
                 const std::string& serviceType,
                 BaseUserLhy* owner);
    virtual ~BaseGroupLhy() = default;

    const std::string& getId() const { return id; }
    const std::string& getName() const { return name; }
    const std::string& getServiceType() const { return serviceType; }
    BaseUserLhy* getOwner() const { return owner; }
    std::vector<BaseUserLhy*> getMemberList() const;
    bool isMember(const std::string& userId) const;
    size_t memberCount() const { return members.size(); }

    bool restoreMember(BaseUserLhy* user);

    virtual bool addMember(BaseUserLhy* user) = 0;
    virtual bool removeMember(const std::string& userId, const std::string& actorId) = 0;
    virtual std::vector<BaseUserLhy*> searchMembers(const std::string& keyword) const;
    virtual bool supportsTempSubGroup() const = 0;
    virtual void showFeatureSummary() const = 0;
};

#endif // BASE_GROUP_LHY_H
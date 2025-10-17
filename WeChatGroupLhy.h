#ifndef WECHAT_GROUP_LHY_H
#define WECHAT_GROUP_LHY_H

#include "BaseGroupLhy.h"

#include <string>

class WeChatGroupLhy : public BaseGroupLhy {
private:
    std::string announcement;
    size_t maxMembers;

public:
    WeChatGroupLhy(const std::string& id, const std::string& name, BaseUserLhy* owner, size_t maxMembers = 500);

    bool addMember(BaseUserLhy* user) override;
    bool removeMember(const std::string& userId, const std::string& actorId) override;
    bool supportsTempSubGroup() const override { return false; }
    void showFeatureSummary() const override;

    void setAnnouncement(const std::string& message);
    const std::string& getAnnouncement() const { return announcement; }
    size_t getMaxMembers() const { return maxMembers; }
};

#endif // WECHAT_GROUP_LHY_H
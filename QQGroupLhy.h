#ifndef QQ_GROUP_LHY_H
#define QQ_GROUP_LHY_H

#include "BaseGroupLhy.h"

#include <map>
#include <set>
#include <string>
#include <vector>

class QQGroupLhy : public BaseGroupLhy {
private:
    std::set<std::string> adminIds;
    std::vector<std::string> tempSubGroupIds;
    std::map<std::string, std::string> joinRequests;  // userId -> requestMessage

public:
    QQGroupLhy(const std::string& id, const std::string& name, BaseUserLhy* owner);

    bool addMember(BaseUserLhy* user) override;
    bool removeMember(const std::string& userId, const std::string& actorId) override;
    bool supportsTempSubGroup() const override { return true; }
    void showFeatureSummary() const override;

    bool addAdmin(const std::string& userId);
    bool removeAdmin(const std::string& userId);
    bool isAdmin(const std::string& userId) const;

    std::string createTempSubGroup(const std::string& name, BaseUserLhy* owner);
    bool dissolveTempSubGroup(const std::string& subGroupId);
    const std::vector<std::string>& getTempSubGroupIds() const { return tempSubGroupIds; }
    void restoreTempSubGroups(const std::vector<std::string>& entries);
    std::vector<std::string> getAdminIds() const;
    
    // 加群申请相关
    bool requestJoin(const std::string& userId, const std::string& message);
    bool approveJoinRequest(const std::string& userId, const std::string& approverId, BaseUserLhy* applicant);
    bool rejectJoinRequest(const std::string& userId, const std::string& rejectorId);
    std::map<std::string, std::string> getJoinRequests() const { return joinRequests; }
};

#endif // QQ_GROUP_LHY_H
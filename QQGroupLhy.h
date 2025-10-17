#ifndef QQ_GROUP_LHY_H
#define QQ_GROUP_LHY_H

#include "BaseGroupLhy.h"

#include <set>
#include <string>
#include <vector>

class QQGroupLhy : public BaseGroupLhy {
private:
    std::set<std::string> adminIds;
    std::vector<std::string> tempSubGroupIds;

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
};

#endif // QQ_GROUP_LHY_H
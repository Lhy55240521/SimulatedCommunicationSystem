#ifndef WECHAT_SERVICE_LHY_H
#define WECHAT_SERVICE_LHY_H

#include "BaseServiceLhy.h"

#include <string>
#include <vector>

class WeChatServiceLhy : public BaseServiceLhy {
private:
    std::vector<std::string> recommendedFriends;

public:
    explicit WeChatServiceLhy(BaseUserLhy* user);

    bool login(const std::string& credential) override;
    void updateRecommendations(const std::vector<std::string>& friends);
    const std::vector<std::string>& getRecommendedFriends() const { return recommendedFriends; }
};

#endif // WECHAT_SERVICE_LHY_H
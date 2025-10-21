#ifndef WEIBO_SERVICE_LHY_H
#define WEIBO_SERVICE_LHY_H

#include "BaseServiceLhy.h"
#include "BaseUserLhy.h"
#include <string>
#include <vector>

class WeiBoServiceLhy : public BaseServiceLhy {
private:
    std::vector<std::string> trendingTopics;  // 热门话题

public:
    // 构造函数
    WeiBoServiceLhy(BaseUserLhy* user);
    
    // 重写基类的虚函数
    bool login(const std::string& credential) override;
    
    // 更新热门话题
    void updateTrendingTopics(const std::vector<std::string>& topics);
    
    // 获取热门话题
    const std::vector<std::string>& getTrendingTopics() const { return trendingTopics; }
    
    // 获取服务类型
    std::string getServiceType() const { return "WEIBO"; }
};

#endif // WEIBO_SERVICE_LHY_H
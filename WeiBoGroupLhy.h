#ifndef WEIBO_GROUP_LHY_H
#define WEIBO_GROUP_LHY_H

#include "BaseGroupLhy.h"
#include <string>
#include <vector>

class WeiBoGroupLhy : public BaseGroupLhy {
private:
    std::string topic;           // 群组话题
    std::vector<std::string> hashtags;  // 标签

public:
    // 构造函数
    WeiBoGroupLhy(const std::string& id, const std::string& name, 
                  BaseUserLhy* owner, const std::string& topic);
    
    // 重写基类的虚函数
    bool addMember(BaseUserLhy* user) override;
    bool removeMember(const std::string& userId, const std::string& actorId) override;
    bool supportsTempSubGroup() const override { return false; }
    void showFeatureSummary() const override;
    
    // 设置话题
    void setTopic(const std::string& newTopic) { topic = newTopic; }
    
    // 获取话题
    const std::string& getTopic() const { return topic; }
    
    // 添加标签
    void addHashtag(const std::string& hashtag);
    
    // 移除标签
    void removeHashtag(const std::string& hashtag);
    
    // 获取所有标签
    const std::vector<std::string>& getHashtags() const { return hashtags; }
    
    // 获取服务类型
    std::string getServiceType() const { return "WEIBO"; }
};

#endif // WEIBO_GROUP_LHY_H
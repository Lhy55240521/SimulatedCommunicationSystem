#ifndef BASE_GROUP_LHY_H
#define BASE_GROUP_LHY_H

#include "BaseUserLhy.h"
#include <map>
#include <vector>
#include <string>

// ������ࣺ����Ⱥ�Ĺ�������
class BaseGroupLhy {
protected:
    std::string id;                      // ȺΨһID��Ԥ�裺1001��1002�ȣ�
    std::string name;                    // Ⱥ����
    BaseUserLhy* owner;                  // Ⱥ�����û�ָ�룩
    std::map<std::string, BaseUserLhy*> members;  // ��Ա�б���key���û�ID��

public:
    // ���캯����Ⱥ��Ĭ�ϼ���Ⱥ��
    BaseGroupLhy(std::string id, std::string name, BaseUserLhy* owner)
        : id(id), name(name), owner(owner) {
        if (owner != nullptr) {
            members[owner->getId()] = owner;
            owner->addGroupId(id);
        }
    }

    // ������������
    virtual ~BaseGroupLhy() = 0;

    // ͨ��Get�ӿ�
    std::string getGroupId() const { return id; }
    std::string getGroupName() const { return name; }
    BaseUserLhy* getGroupOwner() const { return owner; }
    std::vector<BaseUserLhy*> getMembers() const {
        std::vector<BaseUserLhy*> memberList;
        for (const auto& pair : members) {
            memberList.push_back(pair.second);
        }
        return memberList;
    }
    int getMemberCount() const { return members.size(); }

    // Ⱥ��������ӿڣ�������컯ʵ�֣�
    virtual bool addMember(BaseUserLhy* user) = 0;       // ����Ⱥ
    virtual bool deleteMember(std::string userId) = 0;   // �Ƴ�Ⱥ��Ա
    virtual std::vector<BaseUserLhy*> searchMember(std::string keyword = "") = 0;  // ���ҳ�Ա
    virtual bool isTempGroupAllowed() const = 0;         // �Ƿ�������ʱ������

    // ͨ�ýӿڣ��ж��Ƿ�ΪȺ��Ա
    bool isMember(std::string userId) const {
        return members.find(userId) != members.end();
    }
};

// ������������ʵ��
inline BaseGroupLhy::~BaseGroupLhy() = default;

#endif // BASE_GROUP_LHY_H
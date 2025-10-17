#include "UserDataIOLhy.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

// �����������ָ��ַ���
std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

// �����û�����
std::vector<BaseUserLhy*> UserDataIOLhy::loadUsers(const std::string& filename) {
    std::vector<BaseUserLhy*> users;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "[���ݼ���] �û��ļ������ڣ����������ļ���" << filename << std::endl;
        return users;
    }

    std::string line;
    // ������ͷ
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::vector<std::string> data = split(line, '|');
        if (data.size() < 7) continue; // ���ݲ�����������

        std::string type = data[0];
        std::string id = data[1];
        std::string nickname = data[2];
        std::string password = data[3];
        std::string birthDate = data[4];
        int tAge = std::stoi(data[5]);
        std::string location = data[6];
        std::string bindId = (data.size() > 7) ? data[7] : "";

        // ���������б�
        std::vector<std::string> friends;
        if (data.size() > 8) {
            std::vector<std::string> friendList = split(data[8], ',');
            for (const auto& f : friendList) {
                if (!f.empty()) friends.push_back(f);
            }
        }

        BaseUserLhy* user = nullptr;
        if (type == "QQ") {
            user = new QQUserLhy(id, nickname, birthDate, tAge, location, bindId);
            user->changePassword("123", password); // 设置密码
        }
        else if (type == "WECHAT") {
            user = new WeChatUserLhy(id, nickname, birthDate, tAge, location, bindId);
            user->changePassword("123", password); // 设置密码
        }

        if (user) {
            // ���Ӻ���
            for (const auto& f : friends) {
                user->addFriend(f);
            }
            users.push_back(user);
        }
    }

    file.close();
    std::cout << "[���ݼ���] �ɹ����� " << users.size() << " ���û�" << std::endl;
    return users;
}

// �����û�����
void UserDataIOLhy::saveUsers(const std::string& filename, const std::vector<BaseUserLhy*>& users) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "[���ݱ���] �޷����û��ļ���" << filename << std::endl;
        return;
    }

    // д���ͷ
    file << "����|ID|�ǳ�|����|����|T��|���ڵ�|��ID|�����б�\n";

    for (const auto& user : users) {
        if (!user) continue;

        // �ж��û�����
        std::string type = "UNKNOWN";
        std::string bindId = "";

        if (dynamic_cast<QQUserLhy*>(user)) {
            type = "QQ";
            bindId = dynamic_cast<QQUserLhy*>(user)->getBindWeChatId();
        }
        else if (dynamic_cast<WeChatUserLhy*>(user)) {
            type = "WECHAT";
            bindId = dynamic_cast<WeChatUserLhy*>(user)->getBindQQId();
        }

        // ƴ�Ӻ����б�
        std::string friends;
        auto friendIds = user->getFriendIds();
        for (size_t i = 0; i < friendIds.size(); ++i) {
            if (i > 0) friends += ",";
            friends += friendIds[i];
        }

        // д���û�����
        file << type << "|"
            << user->getId() << "|"
            << user->getNickname() << "|"
            << user->getPassword() << "|"  // ��Ҫ��BaseUserLhy������getPassword����
            << user->getBirthDate() << "|"
            << user->getTAge() << "|"
            << user->getLocation() << "|"
            << bindId << "|"
            << friends << "\n";
    }

    file.close();
    std::cout << "[���ݱ���] �ɹ����� " << users.size() << " ���û��� " << filename << std::endl;
}

// ����Ⱥ����
std::vector<BaseGroupLhy*> UserDataIOLhy::loadGroups(const std::string& filename,
    const std::vector<BaseUserLhy*>& allUsers) {
    std::vector<BaseGroupLhy*> groups;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "[���ݼ���] Ⱥ�ļ������ڣ����������ļ���" << filename << std::endl;
        return groups;
    }

    std::string line;
    // ������ͷ
    std::getline(file, line);

    while (std::getline(file, line)) {
        std::vector<std::string> data = split(line, '|');
        if (data.size() < 5) continue; // ���ݲ�����������

        std::string type = data[0];
        std::string groupId = data[1];
        std::string groupName = data[2];
        std::string ownerId = data[3];

        // ����Ⱥ��
        BaseUserLhy* owner = nullptr;
        for (const auto& user : allUsers) {
            if (user && user->getId() == ownerId) {
                owner = user;
                break;
            }
        }

        if (!owner) continue;

        // ����Ⱥ��Ա
        std::vector<BaseUserLhy*> members;
        std::vector<std::string> memberIds = split(data[4], ',');
        for (const auto& id : memberIds) {
            for (const auto& user : allUsers) {
                if (user && user->getId() == id) {
                    members.push_back(user);
                    break;
                }
            }
        }

        // ����Ⱥ�������Ⱥ��Ϣ
        std::string extraInfo = (data.size() > 5) ? data[5] : "";

        BaseGroupLhy* group = nullptr;
        if (type == "QQ") {
            group = new QQGroupLhy(groupId, groupName, owner);
        }
        else if (type == "WECHAT") {
            WeChatGroupLhy* wxGroup = new WeChatGroupLhy(groupId, groupName, owner);
            wxGroup->setAnnouncement(extraInfo); // ����΢��Ⱥ����
            group = wxGroup;
        }

        if (group) {
            // ����Ⱥ��Ա
            for (const auto& member : members) {
                if (member && member->getId() != ownerId) { // Ⱥ�����ڹ��캯��������
                    group->addMember(member);
                }
            }
            groups.push_back(group);
        }
    }

    file.close();
    std::cout << "[���ݼ���] �ɹ����� " << groups.size() << " ��Ⱥ��" << std::endl;
    return groups;
}

// ����Ⱥ����
void UserDataIOLhy::saveGroups(const std::string& filename, const std::vector<BaseGroupLhy*>& groups) {
    std::ofstream file(filename);

    if (!file.is_open()) {
        std::cerr << "[���ݱ���] �޷���Ⱥ�ļ���" << filename << std::endl;
        return;
    }

    // д���ͷ
    file << "����|ȺID|Ⱥ����|Ⱥ��ID|��Ա�б�|������Ϣ(����/��Ⱥ)\n";

    for (const auto& group : groups) {
        if (!group) continue;

        // �ж�Ⱥ����
        std::string type = "UNKNOWN";
        std::string extraInfo = "";

        if (dynamic_cast<QQGroupLhy*>(group)) {
            type = "QQ";
            // ��������������QQȺ���е���Ϣ������Ⱥ��Ϣ
        }
        else if (dynamic_cast<WeChatGroupLhy*>(group)) {
            type = "WECHAT";
            extraInfo = dynamic_cast<WeChatGroupLhy*>(group)->getAnnouncement();
        }

        // ƴ�ӳ�Ա�б�
        std::string members;
        auto memberList = group->getMembers();
        for (size_t i = 0; i < memberList.size(); ++i) {
            if (i > 0) members += ",";
            members += memberList[i]->getId();
        }

        // д��Ⱥ����
        file << type << "|"
            << group->getGroupId() << "|"
            << group->getGroupName() << "|"
            << group->getGroupOwner()->getId() << "|"
            << members << "|"
            << extraInfo << "\n";
    }

    file.close();
    std::cout << "[���ݱ���] �ɹ����� " << groups.size() << " ��Ⱥ�ĵ� " << filename << std::endl;
}

#include "UserDataIOLhy.h"

#include "QQGroupLhy.h"
#include "QQUserLhy.h"
#include "WeChatGroupLhy.h"
#include "WeChatUserLhy.h"

#include <fstream>
#include <iostream>
#include <sstream>

namespace {

std::vector<std::string> split(const std::string& text, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(text);
    std::string item;
    while (std::getline(ss, item, delimiter)) {
        if (!item.empty()) {
            tokens.push_back(item);
        }
    }
    return tokens;
}

std::string join(const std::vector<std::string>& items, char delimiter) {
    std::ostringstream os;
    for (size_t i = 0; i < items.size(); ++i) {
        if (i > 0) {
            os << delimiter;
        }
        os << items[i];
    }
    return os.str();
}

} // namespace

void UserDataIOLhy::loadUsers(const std::string& filename,
                              std::unordered_map<std::string, std::unique_ptr<BaseUserLhy>>& users) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "[DataIO] user file not found: " << filename << " (using defaults)" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        auto fields = split(line, '|');
        if (fields.size() < 8 || (fields[0] != "QQ" && fields[0] != "WECHAT")) {
            continue;
        }

        const std::string& type = fields[0];
        const std::string& id = fields[1];
        const std::string& nickname = fields[2];
        const std::string& password = fields[3];
        const std::string& birth = fields[4];
        int tAge = std::stoi(fields[5]);
        const std::string& location = fields[6];
        const std::string& bindId = fields[7];

        std::unique_ptr<BaseUserLhy> user;
        if (type == "QQ") {
            user = std::make_unique<QQUserLhy>(id, nickname, birth, tAge, location, password, bindId);
        } else if (type == "WECHAT") {
            user = std::make_unique<WeChatUserLhy>(id, nickname, birth, tAge, location, password, bindId);
        } else {
            continue;
        }

        if (fields.size() > 8) {
            for (const auto& service : split(fields[8], ',')) {
                user->openService(service);
            }
        }
        if (fields.size() > 9) {
            for (const auto& friendId : split(fields[9], ',')) {
                user->addFriend(friendId);
            }
        }
        if (fields.size() > 10) {
            for (const auto& remarkEntry : split(fields[10], ',')) {
                auto pos = remarkEntry.find(':');
                if (pos != std::string::npos) {
                    user->setFriendRemark(remarkEntry.substr(0, pos), remarkEntry.substr(pos + 1));
                }
            }
        }
        if (fields.size() > 11) {
            for (const auto& groupId : split(fields[11], ',')) {
                user->addGroupId(groupId);
            }
        }

        users[id] = std::move(user);
    }

    std::cout << "[DataIO] loaded " << users.size() << " users" << std::endl;
}

void UserDataIOLhy::saveUsers(const std::string& filename,
                              const std::unordered_map<std::string, std::unique_ptr<BaseUserLhy>>& users) {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cerr << "[DataIO] cannot open user file for write: " << filename << std::endl;
        return;
    }

    for (const auto& pair : users) {
        const auto* user = pair.second.get();
        if (!user) {
            continue;
        }

        std::string type = "GENERIC";
        std::string bindId;
        if (const auto* qq = dynamic_cast<const QQUserLhy*>(user)) {
            type = "QQ";
            bindId = qq->getBindWeChatId();
        } else if (const auto* wx = dynamic_cast<const WeChatUserLhy*>(user)) {
            type = "WECHAT";
            bindId = wx->getBindQQId();
        }

        std::vector<std::string> services = user->getOpenedServices();
        std::vector<std::string> friends = user->getFriendIds();

        std::vector<std::string> remarks;
        for (const auto& remark : user->getFriendRemarks()) {
            remarks.push_back(remark.first + ":" + remark.second);
        }

        std::vector<std::string> groups = user->getGroupIds();

        fout << type << '|'
             << user->getId() << '|'
             << user->getNickname() << '|'
             << user->getPassword() << '|'
             << user->getBirthDate() << '|'
             << user->getTAge() << '|'
             << user->getLocation() << '|'
             << bindId << '|'
             << join(services, ',') << '|'
             << join(friends, ',') << '|'
             << join(remarks, ',') << '|'
             << join(groups, ',') << '\n';
    }

    std::cout << "[DataIO] saved " << users.size() << " users" << std::endl;
}

void UserDataIOLhy::loadGroups(const std::string& filename,
                               const std::unordered_map<std::string, std::unique_ptr<BaseUserLhy>>& users,
                               std::unordered_map<std::string, std::unique_ptr<BaseGroupLhy>>& groups) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "[DataIO] group file not found: " << filename << " (using defaults)" << std::endl;
        return;
    }

    std::string line;
    while (std::getline(fin, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        auto fields = split(line, '|');
        if (fields.size() < 5 || (fields[0] != "QQ" && fields[0] != "WECHAT")) {
            continue;
        }

        const std::string& type = fields[0];
        const std::string& id = fields[1];
        const std::string& name = fields[2];
        const std::string& ownerId = fields[3];

        auto ownerIt = users.find(ownerId);
        if (ownerIt == users.end()) {
            continue;
        }
        BaseUserLhy* owner = ownerIt->second.get();

        std::unique_ptr<BaseGroupLhy> group;
        if (type == "QQ") {
            group = std::make_unique<QQGroupLhy>(id, name, owner);
        } else if (type == "WECHAT") {
            size_t maxMembers = 500;
            if (fields.size() > 6) {
                maxMembers = static_cast<size_t>(std::stoul(fields[6]));
            }
            group = std::make_unique<WeChatGroupLhy>(id, name, owner, maxMembers);
            if (fields.size() > 5) {
                static_cast<WeChatGroupLhy*>(group.get())->setAnnouncement(fields[5]);
            }
        } else {
            continue;
        }

        if (fields.size() > 4) {
            for (const auto& memberId : split(fields[4], ',')) {
                auto it = users.find(memberId);
                if (it != users.end() && it->second.get() != owner) {
                    group->restoreMember(it->second.get());
                }
            }
        }

        if (type == "QQ") {
            auto* qqGroup = static_cast<QQGroupLhy*>(group.get());
            if (fields.size() > 5) {
                for (const auto& adminId : split(fields[5], ',')) {
                    qqGroup->addAdmin(adminId);
                }
            }
            if (fields.size() > 6) {
                qqGroup->restoreTempSubGroups(split(fields[6], ','));
            }
        }

        groups[id] = std::move(group);
    }

    std::cout << "[DataIO] loaded " << groups.size() << " groups" << std::endl;
}

void UserDataIOLhy::saveGroups(const std::string& filename,
                               const std::unordered_map<std::string, std::unique_ptr<BaseGroupLhy>>& groups) {
    std::ofstream fout(filename);
    if (!fout.is_open()) {
        std::cerr << "[DataIO] cannot open group file for write: " << filename << std::endl;
        return;
    }

    for (const auto& pair : groups) {
        const auto* group = pair.second.get();
        if (!group) {
            continue;
        }

        std::vector<std::string> members;
        for (auto* member : group->getMemberList()) {
            members.push_back(member->getId());
        }

        if (auto* qqGroup = dynamic_cast<const QQGroupLhy*>(group)) {
            std::vector<std::string> admins = qqGroup->getAdminIds();
            std::vector<std::string> tempSubs = qqGroup->getTempSubGroupIds();
            fout << "QQ|"
                 << group->getId() << '|'
                 << group->getName() << '|'
                 << group->getOwner()->getId() << '|'
                 << join(members, ',') << '|'
                 << join(admins, ',') << '|'
                 << join(tempSubs, ',') << '\n';
        } else if (auto* wxGroup = dynamic_cast<const WeChatGroupLhy*>(group)) {
            fout << "WECHAT|"
                 << group->getId() << '|'
                 << group->getName() << '|'
                 << group->getOwner()->getId() << '|'
                 << join(members, ',') << '|'
                 << wxGroup->getAnnouncement() << '|'
                 << wxGroup->getMaxMembers() << '\n';
        }
    }

    std::cout << "[DataIO] saved " << groups.size() << " groups" << std::endl;
}

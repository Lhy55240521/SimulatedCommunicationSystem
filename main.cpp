#include <algorithm>
#include <cctype>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

#include "BaseGroupLhy.h"
#include "BaseServiceLhy.h"
#include "BaseUserLhy.h"
#include "QQGroupLhy.h"
#include "QQServiceLhy.h"
#include "QQUserLhy.h"
#include "UserDataIOLhy.h"
#include "WeChatGroupLhy.h"
#include "WeChatServiceLhy.h"
#include "WeChatUserLhy.h"
#include "WeiBoUserLhy.h"
#include "WeiBoServiceLhy.h"
#include "WeiBoGroupLhy.h"

namespace {

struct PlatformContextLhy {
    std::unordered_map<std::string, std::unique_ptr<BaseUserLhy>> users;
    std::unordered_map<std::string, std::unique_ptr<BaseGroupLhy>> groups;
};

struct ActiveSessionLhy {
    BaseUserLhy* user {nullptr};
    std::unordered_map<std::string, std::unique_ptr<BaseServiceLhy>> services;
    BaseServiceLhy* activeService {nullptr};
};

std::string normalizeService(std::string value) {
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char ch) {
        return static_cast<char>(std::toupper(ch));
    });
    return value;
}

void clearLine() {
    std::cin.clear();
    std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
}

BaseUserLhy* findUser(PlatformContextLhy& ctx, const std::string& id) {
    auto it = ctx.users.find(id);
    return it == ctx.users.end() ? nullptr : it->second.get();
}

BaseGroupLhy* findGroup(PlatformContextLhy& ctx, const std::string& id) {
    auto it = ctx.groups.find(id);
    return it == ctx.groups.end() ? nullptr : it->second.get();
}

void seedUsersIfEmpty(PlatformContextLhy& ctx) {
    if (!ctx.users.empty()) {
        return;
    }
    auto qq = std::make_unique<QQUserLhy>("QQ1001", "QQ先锋", "1999-05-01", "Beijing", "123456", "WX1001");
    auto wx = std::make_unique<WeChatUserLhy>("WX1001", "微信用户", "1998-08-08", "Shanghai", "6666", "QQ1001");
    qq->openService("QQ");
    wx->openService("WECHAT");
    qq->addFriend(wx->getId());
    wx->addFriend(qq->getId());
    ctx.users.emplace(qq->getId(), std::move(qq));
    ctx.users.emplace(wx->getId(), std::move(wx));
    std::cout << "[Seed] 已初始化默认QQ与微信账户" << std::endl;
}

void seedGroupsIfEmpty(PlatformContextLhy& ctx) {
    if (!ctx.groups.empty()) {
        return;
    }
    auto ownerQQ = findUser(ctx, "QQ1001");
    auto ownerWX = findUser(ctx, "WX1001");
    if (!ownerQQ || !ownerWX) {
        return;
    }

    std::vector<std::string> defaultIds = {"1001", "1002", "1003", "1004", "1005", "1006"};
    ctx.groups.emplace("1001", std::make_unique<QQGroupLhy>("1001", "QQ技术交流群", ownerQQ));
    ctx.groups.emplace("1002", std::make_unique<WeChatGroupLhy>("1002", "微信群管理研习社", ownerWX));
    size_t idx = 2;
    for (; idx < defaultIds.size(); ++idx) {
        std::string id = defaultIds[idx];
        if (idx % 2 == 0) {
            ctx.groups.emplace(id, std::make_unique<QQGroupLhy>(id, "QQ特色群" + std::to_string(idx), ownerQQ));
        } else {
            ctx.groups.emplace(id, std::make_unique<WeChatGroupLhy>(id, "微信生活圈" + std::to_string(idx), ownerWX));
        }
    }
    std::cout << "[Seed] 已初始化默认群组 1001-1006" << std::endl;
}

void printUserSummary(const BaseUserLhy* user) {
    std::cout << "\n[账户信息]" << std::endl;
    std::cout << "  ID: " << user->getId() << std::endl;
    std::cout << "  昵称: " << user->getNickname() << std::endl;
    std::cout << "  T龄: " << user->getAccountCreationTime() << std::endl;
    std::cout << "  所在地: " << user->getLocation() << std::endl;
    std::cout << "  已开通服务: ";
    auto services = user->getOpenedServices();
    if (services.empty()) {
        std::cout << "暂无";
    } else {
        for (size_t i = 0; i < services.size(); ++i) {
            if (i) std::cout << ", ";
            std::cout << services[i];
        }
    }
    std::cout << std::endl;
}

ActiveSessionLhy createSession(BaseUserLhy* user, const std::string& requestedType) {
    ActiveSessionLhy session;
    session.user = user;
    std::string primary = normalizeService(requestedType);

    std::vector<std::string> services = user->getOpenedServices();
    services.push_back(primary);
    std::sort(services.begin(), services.end());
    services.erase(std::unique(services.begin(), services.end()), services.end());

    for (const auto& type : services) {
        auto service = BaseServiceLhy::create(type, user);
        if (!service) {
            continue;
        }
        BaseServiceLhy* raw = service.get();
        session.services.emplace(normalizeService(type), std::move(service));
        if (normalizeService(type) == primary) {
            session.activeService = raw;
        }
    }
    if (!session.activeService) {
        auto primaryService = BaseServiceLhy::create(primary, user);
        session.activeService = primaryService.get();
        session.services.emplace(primary, std::move(primaryService));
    }
    return session;
}

void refreshRecommendations(PlatformContextLhy& ctx, ActiveSessionLhy& session) {
    auto it = session.services.find("WECHAT");
    if (it == session.services.end()) {
        return;
    }
    auto* wxService = dynamic_cast<WeChatServiceLhy*>(it->second.get());
    if (!wxService) {
        return;
    }
    std::vector<std::string> recommended;

    if (auto* wxUser = dynamic_cast<WeChatUserLhy*>(session.user)) {
        const std::string& qqId = wxUser->getBindQQId();
        if (!qqId.empty()) {
            if (auto* qqUser = findUser(ctx, qqId)) {
                recommended = qqUser->getFriendIds();
            }
        }
    } else if (auto* qqUser = dynamic_cast<QQUserLhy*>(session.user)) {
        const std::string& wxId = qqUser->getBindWeChatId();
        if (!wxId.empty()) {
            if (auto* another = findUser(ctx, wxId)) {
                recommended = another->getFriendIds();
            }
        }
    }
    wxService->updateRecommendations(recommended);
}

void showFriendMenu() {
    std::cout << "\n----- 好友管理 -----" << std::endl;
    std::cout << "1. 查看好友列表" << std::endl;
    std::cout << "2. 添加好友" << std::endl;
    std::cout << "3. 删除好友" << std::endl;
    std::cout << "4. 更新好友备注" << std::endl;
    std::cout << "5. 查看跨服务推荐好友" << std::endl;
    std::cout << "0. 返回" << std::endl;
    std::cout << "请选择：";
}

void manageFriends(PlatformContextLhy& ctx, ActiveSessionLhy& session) {
    refreshRecommendations(ctx, session);
    while (true) {
        showFriendMenu();
        int choice = 0;
        std::cin >> choice;
        if (std::cin.fail()) { 
            std::cin.clear(); // 清除错误状态
            clearLine(); 
            std::cout << "[提示] 请输入有效数字选项" << std::endl;
            continue; 
        }
        if (choice == 0) {
            return;
        }

        auto* user = session.user;
        switch (choice) {
        case 1: {
            auto friends = user->getFriendIds();
            std::cout << "\n[好友列表] 共 " << friends.size() << " 人" << std::endl;
            for (const auto& fid : friends) {
                auto remarkIt = user->getFriendRemarks().find(fid);
                std::cout << "  - " << fid;
                if (remarkIt != user->getFriendRemarks().end()) {
                    std::cout << " (备注: " << remarkIt->second << ")";
                }
                std::cout << std::endl;
            }
            break;
        }
        case 2: {
            std::string target;
            std::cout << "输入好友ID：";
            std::cin >> target;
            auto* other = findUser(ctx, target);
            if (!other) {
                std::cout << "[提示] 用户不存在" << std::endl;
                break;
            }
            if (user->addFriend(target)) {
                other->addFriend(user->getId());
                std::cout << "[成功] 已互为好友" << std::endl;
            } else {
                std::cout << "[提示] 已经是好友" << std::endl;
            }
            break;
        }
        case 3: {
            std::string target;
            std::cout << "输入需要删除的好友ID：";
            std::cin >> target;
            if (user->removeFriend(target)) {
                if (auto* other = findUser(ctx, target)) {
                    other->removeFriend(user->getId());
                }
                std::cout << "[成功] 好友已删除" << std::endl;
            } else {
                std::cout << "[提示] 非好友" << std::endl;
            }
            break;
        }
        case 4: {
            std::string target; std::string remark;
            std::cout << "输入好友ID：";
            std::cin >> target;
            std::cout << "输入备注(为空则清除)：";
            clearLine();
            std::getline(std::cin, remark);
            user->setFriendRemark(target, remark);
            std::cout << "[成功] 备注已更新" << std::endl;
            break;
        }
        case 5: {
            auto svcIt = session.services.find("WECHAT");
            if (svcIt == session.services.end()) {
                std::cout << "[提示] 当前无微信服务" << std::endl;
                break;
            }
            auto* wxService = dynamic_cast<WeChatServiceLhy*>(svcIt->second.get());
            const auto& rec = wxService->getRecommendedFriends();
            if (rec.empty()) {
                std::cout << "[提示] 暂无推荐好友" << std::endl;
            } else {
                std::cout << "[推荐好友]" << std::endl;
                for (const auto& id : rec) {
                    std::cout << "  - " << id << std::endl;
                }
            }
            break;
        }
        default:
            std::cout << "[提示] 请选择有效选项" << std::endl;
            break;
        }
    }
}

void showGroupMenu(const std::string& serviceType) {
    std::cout << "\n----- 群组管理 -----" << std::endl;
    std::cout << "1. 查看全部群" << std::endl;
    std::cout << "2. 加入群" << std::endl;
    std::cout << "3. 退出群" << std::endl;
    std::cout << "4. 挨T成员" << std::endl;
    
    // 根据当前服务类型显示相应的特有功能
    if (serviceType == "QQ") {
        std::cout << "5. 创建QQ临时讨论组" << std::endl;
        std::cout << "6. 解散QQ临时讨论组" << std::endl;
    } else if (serviceType == "WECHAT") {
        std::cout << "5. 设置微信群公告" << std::endl;
    }
    
    std::cout << "7. 切换群类型" << std::endl;
    std::cout << "0. 返回" << std::endl;
    std::cout << "请选择：";
}

void convertGroup(PlatformContextLhy& ctx, const std::string& groupId, const std::string& targetType) {
    auto* oldGroup = findGroup(ctx, groupId);
    if (!oldGroup) {
        std::cout << "[提示] 群不存在" << std::endl;
        return;
    }
    auto owner = oldGroup->getOwner();
    auto members = oldGroup->getMemberList();

    std::unique_ptr<BaseGroupLhy> replacement;
    std::string normalized = normalizeService(targetType);
    if (normalized == "QQ") {
        replacement = std::make_unique<QQGroupLhy>(oldGroup->getId(), oldGroup->getName(), owner);
    } else if (normalized == "WECHAT") {
        replacement = std::make_unique<WeChatGroupLhy>(oldGroup->getId(), oldGroup->getName(), owner);
    } else {
        std::cout << "[提示] 不支持的目标类型" << std::endl;
        return;
    }

    for (auto* member : members) {
        if (member != owner) {
            replacement->restoreMember(member);
        }
    }
    ctx.groups[oldGroup->getId()] = std::move(replacement);
    std::cout << "[成功] 群 " << groupId << " 已转换为 " << normalized << " 类型" << std::endl;
}

void manageGroups(PlatformContextLhy& ctx, ActiveSessionLhy& session) {
    // 获取当前活动服务的类型
    std::string currentServiceType = session.activeService ? session.activeService->getServiceType() : "";
    
    while (true) {
        showGroupMenu(currentServiceType);
        int choice = 0;
        std::cin >> choice;
        if (std::cin.fail()) { 
            std::cin.clear(); // 清除错误状态
            clearLine(); 
            std::cout << "[提示] 请输入有效数字选项" << std::endl;
            continue; 
        }
        if (choice == 0) {
            return;
        }

        switch (choice) {
        case 1: {
            std::cout << "\n[群组列表]" << std::endl;
            for (const auto& pair : ctx.groups) {
                const auto* group = pair.second.get();
                std::cout << "  - " << group->getId() << " [" << group->getServiceType()
                          << "] " << group->getName() << " 人数: " << group->memberCount() << std::endl;
                group->showFeatureSummary();
            }
            break;
        }
        case 2: {
            std::string id;
            std::cout << "输入要加入的群ID：";
            std::cin >> id;
            auto* group = findGroup(ctx, id);
            if (!group) {
                std::cout << "[提示] 群不存在" << std::endl;
                break;
            }
            if (group->addMember(session.user)) {
                std::cout << "[成功] 已加入群" << std::endl;
            } else {
                std::cout << "[提示] 加入失败" << std::endl;
            }
            break;
        }
        case 3: {
            std::string id;
            std::cout << "输入要退出的群ID：";
            std::cin >> id;
            auto* group = findGroup(ctx, id);
            if (!group) {
                std::cout << "[提示] 群不存在" << std::endl;
                break;
            }
            if (group->removeMember(session.user->getId(), session.user->getId())) {
                std::cout << "[成功] 已退出群" << std::endl;
            } else {
                std::cout << "[提示] 无法退出，可能是群主" << std::endl;
            }
            break;
        }
        case 4: {
            std::string id, target;
            std::cout << "输入群ID：";
            std::cin >> id;
            auto* group = findGroup(ctx, id);
            if (!group) { std::cout << "[提示] 群不存在" << std::endl; break; }
            std::cout << "输入要移除的成员ID：";
            std::cin >> target;
            if (group->removeMember(target, session.user->getId())) {
                std::cout << "[成功] 已移除成员" << std::endl;
            } else {
                std::cout << "[提示] 无权限或成员不存在" << std::endl;
            }
            break;
        }
        case 5: {
            // 根据当前服务类型处理不同的功能
            if (currentServiceType == "QQ") {
                // 创建QQ临时讨论组
                std::string id, name;
                std::cout << "输入QQ群ID：";
                std::cin >> id;
                auto* group = dynamic_cast<QQGroupLhy*>(findGroup(ctx, id));
                if (!group) { std::cout << "[提示] 不是QQ群" << std::endl; break; }
                std::cout << "输入临时讨论组名称：";
                std::cin >> name;
                group->createTempSubGroup(name, session.user);
            } else if (currentServiceType == "WECHAT") {
                // 设置微信群公告
                std::string id;
                std::cout << "输入微信群ID：";
                std::cin >> id;
                auto* group = dynamic_cast<WeChatGroupLhy*>(findGroup(ctx, id));
                if (!group) { std::cout << "[提示] 不是微信群" << std::endl; break; }
                if (group->getOwner()->getId() != session.user->getId()) {
                    std::cout << "[提示] 只有群主可设置公告" << std::endl;
                    break;
                }
                std::string announcement;
                std::cout << "输入公告内容：";
                clearLine();
                std::getline(std::cin, announcement);
                group->setAnnouncement(announcement);
            } else {
                std::cout << "[提示] 当前服务不支持此功能" << std::endl;
            }
            break;
        }
        case 6: {
            // 解散QQ临时讨论组（仅QQ服务）
            if (currentServiceType == "QQ") {
                std::string id, tempId;
                std::cout << "输入QQ群ID：";
                std::cin >> id;
                auto* group = dynamic_cast<QQGroupLhy*>(findGroup(ctx, id));
                if (!group) { std::cout << "[提示] 不是QQ群" << std::endl; break; }
                std::cout << "输入临时讨论组ID（如1001_temp_1）：";
                std::cin >> tempId;
                group->dissolveTempSubGroup(tempId);
            } else {
                std::cout << "[提示] 当前服务不支持此功能" << std::endl;
            }
            break;
        }
        case 7: {
            std::string id, type;
            std::cout << "输入群ID：";
            std::cin >> id;
            std::cout << "转换为类型(QQ/WECHAT)：";
            std::cin >> type;
            convertGroup(ctx, id, type);
            break;
        }
        default:
            std::cout << "[提示] 请选择有效选项" << std::endl;
            break;
        }
    }
}

void showServiceMenu() {
    std::cout << "\n----- 服务管理 -----" << std::endl;
    std::cout << "1. 查看服务状态" << std::endl;
    std::cout << "2. 开通新服务" << std::endl;
    std::cout << "3. 切换活动服务" << std::endl;
    std::cout << "0. 返回" << std::endl;
    std::cout << "请选择：";
}

void manageServices(ActiveSessionLhy& session) {
    while (true) {
        showServiceMenu();
        int choice = 0;
        std::cin >> choice;
        if (std::cin.fail()) { 
            std::cin.clear(); // 清除错误状态
            clearLine(); 
            std::cout << "[提示] 请输入有效数字选项" << std::endl;
            continue; 
        }
        if (choice == 0) {
            return;
        }

        switch (choice) {
        case 1: {
            std::cout << "\n[服务状态]" << std::endl;
            for (const auto& pair : session.services) {
                std::cout << "  - " << pair.first << (pair.second->isLoggedIn() ? " 已登录" : " 未登录");
                if (pair.second.get() == session.activeService) {
                    std::cout << " (当前)";
                }
                std::cout << std::endl;
            }
            break;
        }
        case 2: {
            std::string type;
            std::cout << "输入服务类型(QQ/WECHAT/WEIBO)：";
            std::cin >> type;
            type = normalizeService(type);
            if (session.services.count(type)) {
                std::cout << "[提示] 服务已开通" << std::endl;
                break;
            }
            auto service = BaseServiceLhy::create(type, session.user);
            if (!service) {
                std::cout << "[提示] 创建失败" << std::endl;
                break;
            }
            std::string credential;
            std::cout << "输入登录凭证：";
            std::cin >> credential;
            if (!service->login(credential)) {
                std::cout << "[提示] 凭证错误" << std::endl;
                break;
            }
            session.user->openService(type);
            session.activeService = service.get();
            session.services.emplace(type, std::move(service));
            std::cout << "[成功] 服务已开通并登录" << std::endl;
            break;
        }
        case 3: {
            std::string type;
            std::cout << "输入要切换的服务类型：";
            std::cin >> type;
            type = normalizeService(type);
            auto it = session.services.find(type);
            if (it == session.services.end()) {
                std::cout << "[提示] 服务未开通" << std::endl;
                break;
            }
            session.activeService = it->second.get();
            std::cout << "[成功] 已切换到 " << type << " 服务" << std::endl;
            break;
        }
        default:
            std::cout << "[提示] 请选择有效选项" << std::endl;
            break;
        }
    }
}

void simulateQQChat(ActiveSessionLhy& session) {
    auto it = session.services.find("QQ");
    if (it == session.services.end()) {
        std::cout << "[提示] 当前未开通QQ服务" << std::endl;
        return;
    }
    auto* qqService = dynamic_cast<QQServiceLhy*>(it->second.get());
    if (!qqService) {
        std::cout << "[提示] 服务异常" << std::endl;
        return;
    }

    while (true) {
        std::cout << "\n----- QQ消息模拟 -----" << std::endl;
        std::cout << "1. 发送消息" << std::endl;
        std::cout << "2. 模拟接收" << std::endl;
        std::cout << "3. 查看收件箱" << std::endl;
        std::cout << "0. 返回" << std::endl;
        std::cout << "请选择：";

        int choice = 0;
        std::cin >> choice;
        if (std::cin.fail()) { 
            std::cin.clear(); // 清除错误状态
            clearLine(); 
            std::cout << "[提示] 请输入有效数字选项" << std::endl;
            continue; 
        }
        
        if (choice == 0) {
            return;
        }

        switch (choice) {
        case 1: {
            std::string peer, message;
            std::cout << "输入对方ID：";
            std::cin >> peer;
            std::cout << "输入消息：";
            clearLine();
            std::getline(std::cin, message);
            qqService->enqueueOutgoingMessage(peer, message);
            break;
        }
        case 2: {
            std::string peer, message;
            std::cout << "模拟来自ID：";
            std::cin >> peer;
            std::cout << "消息内容：";
            clearLine();
            std::getline(std::cin, message);
            qqService->emulateIncomingMessage(peer, message);
            break;
        }
        case 3: {
            std::cout << qqService->dequeueIncomingMessage() << std::endl;
            break;
        }
        default:
            std::cout << "[提示] 请选择有效选项" << std::endl;
            break;
        }
    }
}

void sessionLoop(PlatformContextLhy& ctx, ActiveSessionLhy& session) {
    bool running = true;
    while (running) {
        std::cout << "\n===== 服务控制台 =====" << std::endl;
        std::cout << "1. 好友管理" << std::endl;
        std::cout << "2. 群组管理" << std::endl;
        std::cout << "3. 服务开通管理" << std::endl;
        std::cout << "4. 查看账户信息" << std::endl;
        std::cout << "5. QQ消息模拟" << std::endl;
        std::cout << "0. 登出" << std::endl;
        std::cout << "请选择：";

        int choice = 0;
        std::cin >> choice;
        if (std::cin.fail()) { 
            std::cin.clear(); // 清除错误状态
            clearLine(); 
            std::cout << "[提示] 请输入有效数字选项" << std::endl;
            continue; 
        }

        switch (choice) {
        case 1:
            manageFriends(ctx, session);
            break;
        case 2:
            manageGroups(ctx, session);
            break;
        case 3:
            manageServices(session);
            break;
        case 4:
            printUserSummary(session.user);
            break;
        case 5:
            simulateQQChat(session);
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "[提示] 请选择有效选项" << std::endl;
            break;
        }
    }

    for (auto& pair : session.services) {
        pair.second->logout();
    }
}

void registerUser(PlatformContextLhy& ctx, const std::string& type) {
    std::string id, nickname, birth, location, password;
    std::cout << "输入ID：";
    std::cin >> id;
    if (ctx.users.count(id)) {
        std::cout << "[提示] ID已存在" << std::endl;
        return;
    }
    std::cout << "输入昵称：";
    std::cin >> nickname;
    std::cout << "输入出生日期(YYYY-MM-DD)：";
    std::cin >> birth;
    std::cout << "输入所在地：";
    std::cin >> location;
    std::cout << "设置密码：";
    std::cin >> password;

    std::unique_ptr<BaseUserLhy> user;
    std::string normalized = normalizeService(type);
    if (normalized == "QQ") {
        std::string bindId;
        std::cout << "可选绑定微信ID（没有请输入-）：";
        std::cin >> bindId;
        user = std::make_unique<QQUserLhy>(id, nickname, birth, location, password, bindId == "-" ? "" : bindId);
    } else if (normalized == "WECHAT") {
        std::string bindId;
        std::cout << "可选绑定QQ ID（没有请输入-）：";
        std::cin >> bindId;
        user = std::make_unique<WeChatUserLhy>(id, nickname, birth, location, password, bindId == "-" ? "" : bindId);
    } else if (normalized == "WEIBO") {
        // 微博服务必须绑定到已存在的QQ账号
        if (!ctx.users.count(id)) {
            std::cout << "[提示] 该ID不存在，请先注册QQ账号" << std::endl;
            return;
        }
        
        // 获取已存在的用户
        auto& existingUser = ctx.users.at(id);
        QQUserLhy* qqUserPtr = dynamic_cast<QQUserLhy*>(existingUser.get());
        if (!qqUserPtr) {
            std::cout << "[提示] 该ID不是QQ账号，无法注册微博服务" << std::endl;
            return;
        }
        
        // 使用QQ用户的信息创建微博用户（自动填充QQ用户的信息）
        user = std::make_unique<WeiBoUserLhy>(id, qqUserPtr->getNickname(), qqUserPtr->getBirthDate(), 
                                             qqUserPtr->getLocation(), password, id);
    } else {
        std::cout << "[提示] 不支持的用户类型" << std::endl;
        return;
    }
    user->openService(normalized);
    ctx.users.emplace(id, std::move(user));
    std::cout << "[成功] 注册完成" << std::endl;
}

void loginFlow(PlatformContextLhy& ctx, const std::string& type) {
    std::string id;
    std::cout << "输入账号ID：";
    std::cin >> id;
    auto* user = findUser(ctx, id);
    if (!user) {
        std::cout << "[提示] 用户不存在" << std::endl;
        return;
    }

    std::string credential;
    std::cout << "输入登录凭证：";
    std::cin >> credential;

    ActiveSessionLhy session = createSession(user, type);
    if (!session.activeService) {
        std::cout << "[提示] 无法创建服务" << std::endl;
        return;
    }
    if (!session.activeService->login(credential)) {
        std::cout << "[提示] 登录失败" << std::endl;
        return;
    }
    sessionLoop(ctx, session);
}

void saveAll(const PlatformContextLhy& ctx, const std::string& userFile, const std::string& groupFile) {
    UserDataIOLhy::saveUsers(userFile, ctx.users);
    UserDataIOLhy::saveGroups(groupFile, ctx.groups);
}

void loadAll(PlatformContextLhy& ctx, const std::string& userFile, const std::string& groupFile) {
    UserDataIOLhy::loadUsers(userFile, ctx.users);
    UserDataIOLhy::loadGroups(groupFile, ctx.users, ctx.groups);
}

void showMainMenu() {
    std::cout << "\n===== 统一社交平台 =====" << std::endl;
    std::cout << "1. QQ服务登录" << std::endl;
    std::cout << "2. 微信服务登录" << std::endl;
    std::cout << "3. 注册QQ账号" << std::endl;
    std::cout << "4. 注册微信账号" << std::endl;
    std::cout << "5. 注册微博账号" << std::endl;
    std::cout << "6. 微博服务登录" << std::endl;
    std::cout << "7. 浏览群组概览" << std::endl;
    std::cout << "0. 退出" << std::endl;
    std::cout << "请选择：";
}

void showGroupOverview(PlatformContextLhy& ctx) {
    std::cout << "\n[群组概览]" << std::endl;
    for (const auto& pair : ctx.groups) {
        const auto* group = pair.second.get();
        std::cout << "  - ID:" << group->getId() << " 类型:" << group->getServiceType()
                  << " 名称:" << group->getName() << " 成员:" << group->memberCount() << std::endl;
    }
}

} // namespace

int main() {
#ifdef _WIN32
    SetConsoleOutputCP(65001);
    SetConsoleCP(65001);
#endif

    PlatformContextLhy ctx;
    const std::string userFile = "users_lhy.txt";
    const std::string groupFile = "groups_lhy.txt";

    loadAll(ctx, userFile, groupFile);
    seedUsersIfEmpty(ctx);
    seedGroupsIfEmpty(ctx);

    bool running = true;
    while (running) {
        showMainMenu();
        int choice = 0;
        
        // 改进的输入处理逻辑，防止死循环
        if (!(std::cin >> choice)) {
            // 输入失败，清除错误状态和缓冲区
            std::cin.clear();
            std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');
            std::cout << "[提示] 请输入有效数字选项" << std::endl;
            continue;
        }
        
        // 清除输入缓冲区中的剩余字符
        std::cin.ignore((std::numeric_limits<std::streamsize>::max)(), '\n');

        switch (choice) {
        case 1:
            loginFlow(ctx, "QQ");
            break;
        case 2:
            loginFlow(ctx, "WECHAT");
            break;
        case 3:
            registerUser(ctx, "QQ");
            break;
        case 4:
            registerUser(ctx, "WECHAT");
            break;
        case 5:
            registerUser(ctx, "WEIBO");
            break;
        case 6:
            loginFlow(ctx, "WEIBO");
            break;
        case 7:
            showGroupOverview(ctx);
            break;
        case 0:
            running = false;
            break;
        default:
            std::cout << "[提示] 请选择有效选项" << std::endl;
            break;
        }
    }

    saveAll(ctx, userFile, groupFile);
    std::cout << "[系统] 数据已保存，欢迎下次使用！" << std::endl;
    return 0;
}
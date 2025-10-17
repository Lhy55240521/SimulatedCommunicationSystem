#include <iostream>
#include <vector>
#include <windows.h>
#include "BaseUserLhy.h"
#include "BaseGroupLhy.h"
#include "BaseServiceLhy.h"
#include "UserDataIOLhy.h"
#include "QQUserLhy.h"
#include "WeChatUserLhy.h"
#include "QQGroupLhy.h"
#include "WeChatGroupLhy.h"
#include "QQServiceLhy.h"
#include "WeChatServiceLhy.h"

// 全局函数：显示入口选择菜单
void showEntryMenu() {
    std::cout << "\n===== 模拟即时通信系统（Lhy）=====" << std::endl;
    std::cout << "请选择服务入口：" << std::endl;
    std::cout << "1. QQ 服务" << std::endl;
    std::cout << "2. 微信 服务" << std::endl;
    std::cout << "3. 退出系统" << std::endl;
    std::cout << "==================================" << std::endl;
    std::cout << "请输入选择编号（1-3）：";
}

// 全局函数：显示QQ专属功能菜单
void showQQMenu() {
    std::cout << "\n===== QQ 功能菜单 =====" << std::endl;
    std::cout << "1. 好友管理（添加/删除/查询）" << std::endl;
    std::cout << "2. 群组管理（加入/退出/查询成员/创建临时讨论组）" << std::endl;
    std::cout << "3. QQ-TCP点对点通信" << std::endl;
    std::cout << "4. 退出登录" << std::endl;
    std::cout << "========================" << std::endl;
    std::cout << "请输入选择编号（1-4）：";
}

// 全局函数：显示微信专属功能菜单
void showWeChatMenu() {
    std::cout << "\n===== 微信 功能菜单 =====" << std::endl;
    std::cout << "1. 好友管理（添加/删除/查询）" << std::endl;
    std::cout << "2. 群组管理（加入/退出/查询成员/设置公告）" << std::endl;
    std::cout << "3. 微信-QQ互联" << std::endl;
    std::cout << "4. 退出登录" << std::endl;
    std::cout << "==========================" << std::endl;
    std::cout << "请输入选择编号（1-4）：";
}

// 全局函数：显示群组子菜单（用于扩展原逻辑实现双循环）
void showGroupSubMenu() {
    std::cout << "\n----- 群组操作子菜单 -----" << std::endl;
    std::cout << "1. 加入群" << std::endl;
    std::cout << "2. 退出群" << std::endl;
    std::cout << "3. 查询群成员" << std::endl;
    std::cout << "4. 群角色管理" << std::endl;
    std::cout << "5. 返回上一级" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "请输入选择编号（1-5）：";
}

// 辅助函数：查找用户
BaseUserLhy* findUser(const std::vector<BaseUserLhy*>& allUsers, const std::string& userId) {
    for (auto user : allUsers) {
        if (user->getId() == userId) {
            return user;
        }
    }
    return nullptr;
}

// 辅助函数：QQ用户注册
BaseUserLhy* registerQQUser() {
    std::string id, nickname, birthDate, location;
    int tAge;
    std::cout << "\n===== QQ 用户注册 =====" << std::endl;
    std::cout << "请输入QQ ID（以QQ开头，如QQ123）：";
    std::cin >> id;
    std::cout << "请输入昵称：";
    std::cin >> nickname;
    std::cout << "请输入出生时间（格式：YYYY-MM-DD）：";
    std::cin >> birthDate;
    std::cout << "请输入T龄（从注册时开始计算，单位：年）：";
    std::cin >> tAge;
    std::cout << "请输入所在地：";
    std::cin >> location;
    return new QQUserLhy(id, nickname, birthDate, tAge, location);
}

// 辅助函数：微信用户注册
BaseUserLhy* registerWeChatUser() {
    std::string id, nickname, birthDate, location;
    int tAge;
    std::cout << "\n===== 微信 用户注册 =====" << std::endl;
    std::cout << "请输入微信ID（以WX开头，如WX456）：";
    std::cin >> id;
    std::cout << "请输入昵称：";
    std::cin >> nickname;
    std::cout << "请输入出生时间（格式：YYYY-MM-DD）：";
    std::cin >> birthDate;
    std::cout << "请输入T龄（从注册时开始计算，单位：年）：";
    std::cin >> tAge;
    std::cout << "请输入所在地：";
    std::cin >> location;
    return new WeChatUserLhy(id, nickname, birthDate, tAge, location);
}

// 辅助函数：显示个人信息管理菜单
void showProfileMenu() {
    std::cout << "\n----- 个人信息管理 -----" << std::endl;
    std::cout << "1. 修改密码" << std::endl;
    std::cout << "2. 修改昵称" << std::endl;
    std::cout << "3. 修改生日" << std::endl;
    std::cout << "4. 修改所在地" << std::endl;
    std::cout << "5. 返回上一级" << std::endl;
    std::cout << "------------------------" << std::endl;
    std::cout << "请输入选择编号（1-5）：";
}

// 主函数
int main() {
    // 设置控制台编码为GBK，解决中文乱码问题
    SetConsoleOutputCP(936);
    SetConsoleCP(936);
    
    // 1. 初始化数据文件（加载用户和群组数据）
    const std::string USER_FILE = "users_lhy.txt";
    const std::string GROUP_FILE = "groups_lhy.txt";
    std::vector<BaseUserLhy*> allUsers = UserDataIOLhy::loadUsers(USER_FILE);
    std::vector<BaseGroupLhy*> allGroups = UserDataIOLhy::loadGroups(GROUP_FILE, allUsers);

    // 2. 初始化测试数据（如果文件无数据）
    if (allUsers.empty()) {
        std::cout << "\n[初始化] 检测到无用户数据，创建测试用户" << std::endl;
        BaseUserLhy* qqTestUser = new QQUserLhy("QQ123", "测试用户", "2000-01-01", 5, "北京");
        BaseUserLhy* wxTestUser = new WeChatUserLhy("WX456", "微信用户", "1999-02-02", 6, "上海");
        qqTestUser->addFriend("WX456");
        wxTestUser->addFriend("QQ123");
        allUsers.push_back(qqTestUser);
        allUsers.push_back(wxTestUser);
        std::cout << "  - QQ123（测试用户）添加微信WX456为好友" << std::endl;
        std::cout << "  - WX456（微信用户）添加QQ123为好友" << std::endl;
    }
    if (allGroups.empty()) {
        std::cout << "\n[初始化] 检测到无群组数据，创建测试群组" << std::endl;
        if (!allUsers.empty()) {
            BaseGroupLhy* qqTestGroup = new QQGroupLhy("1001", "QQ技术交流群", allUsers[0]);
            BaseGroupLhy* wxTestGroup = new WeChatGroupLhy("1002", "微信学习群", allUsers[1]);
            allGroups.push_back(qqTestGroup);
            allGroups.push_back(wxTestGroup);
            std::cout << "  - QQ群1001（QQ技术交流群），群主：" << allUsers[0]->getNickname() << std::endl;
            std::cout << "  - 微信群1002（微信学习群），群主：" << allUsers[1]->getNickname() << std::endl;
        }
    }

    // 3. 主循环：入口选择
    int entryChoice;
    while (true) {
        showEntryMenu();
        std::cin >> entryChoice;

        // 输入合法性检查
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "[输入错误] 请输入1-3之间的数字" << std::endl;
            continue;
        }

        switch (entryChoice) {
            // QQ服务
        case 1: {
            int qqOp;
            std::cout << "\n===== QQ 服务 =====" << std::endl;
            std::cout << "1. 登录" << std::endl;
            std::cout << "2. 注册" << std::endl;
            std::cout << "请选择操作（1-2）：";
            std::cin >> qqOp;

            BaseUserLhy* qqUser = nullptr;
            BaseServiceLhy* qqService = nullptr;

            if (qqOp == 1) {
                // QQ登录
                std::string userId, password;
                std::cout << "请输入QQ ID（测试ID：QQ123）：";
                std::cin >> userId;
                std::cout << "请输入密码（测试密码：123）：";
                std::cin >> password;

                qqUser = findUser(allUsers, userId);
                if (!qqUser) {
                    std::cout << "[登录失败] 用户不存在" << std::endl;
                    break;
                }

                qqService = BaseServiceLhy::createService("QQ", qqUser);
                if (!qqService || !qqService->login(password)) {
                    delete qqService;
                    break;
                }
                qqService->syncLogin();
            }
            else if (qqOp == 2) {
                // QQ注册
                qqUser = registerQQUser();
                allUsers.push_back(qqUser);
                std::cout << "[注册成功] 请登录" << std::endl;

                std::string password = "123";  // 新用户默认密码
                qqService = BaseServiceLhy::createService("QQ", qqUser);
                qqService->login(password);
                qqService->syncLogin();
            }
            else {
                std::cout << "[输入错误] 请输入1或2" << std::endl;
                break;
            }

            // QQ功能循环
            int qqChoice, subChoice;
            while (qqService && qqService->getIsLoggedIn()) {
                showQQMenu();
                std::cin >> qqChoice;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "[输入错误] 请输入1-4之间的数字" << std::endl;
                    continue;
                }

                switch (qqChoice) {
                    // QQ好友管理
                case 1: {
                    BaseUserLhy* currentUser = qqService->getUser();
                    std::string friendId;
                    std::cout << "\n当前用户：" << currentUser->getNickname() << "（" << currentUser->getId() << "）" << std::endl;
                    std::cout << "请选择好友操作：1-添加，2-删除，3-查询：";
                    int friendOp;
                    std::cin >> friendOp;

                    switch (friendOp) {
                    case 1:
                        std::cout << "请输入好友ID：";
                        std::cin >> friendId;
                        currentUser->addFriend(friendId) ? std::cout << "添加成功" << std::endl : std::cout << "添加失败" << std::endl;
                        break;
                    case 2:
                        std::cout << "请输入好友ID：";
                        std::cin >> friendId;
                        currentUser->deleteFriend(friendId) ? std::cout << "删除成功" << std::endl : std::cout << "删除失败" << std::endl;
                        break;
                    case 3: {
                        auto friends = currentUser->getFriendIds();
                        std::cout << "好友列表（共" << friends.size() << "人）：" << std::endl;
                        for (const auto& id : friends) std::cout << "  - " << id << std::endl;
                        break;
                    }
                    default:
                        std::cout << "[输入错误] 请输入1-3之间的数字" << std::endl;
                    }
                    break;
                }
                      // QQ群组管理
                case 2: {
                    while (true) {
                        showGroupSubMenu();
                        std::cin >> subChoice;

                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(1000, '\n');
                            std::cout << "[输入错误] 请输入1-5之间的数字" << std::endl;
                            continue;
                        }

                        QQServiceLhy* qqSvc = dynamic_cast<QQServiceLhy*>(qqService);
                        BaseUserLhy* currentUser = qqService->getUser();

                        switch (subChoice) {
                        case 1: {
                            std::string groupId;
                            std::cout << "请输入QQ群ID（测试群：1001）：";
                            std::cin >> groupId;
                            BaseGroupLhy* group = nullptr;
                            for (auto g : allGroups) if (g->getGroupId() == groupId) group = g;
                            if (group && qqSvc) qqSvc->joinGroup(dynamic_cast<QQGroupLhy*>(group));
                            else std::cout << "群不存在或服务异常" << std::endl;
                            break;
                        }
                        case 2: {
                            std::string groupId;
                            std::cout << "请输入QQ群ID：";
                            std::cin >> groupId;
                            if (qqSvc != nullptr) {
                                // QQ服务存在，执行退出群操作
                                qqSvc->quitGroup(groupId);
                            }
                            else {
                                // QQ服务未初始化，显示异常
                                std::cout << "服务异常" << std::endl;
                            }
                            break;
                        }
                        case 3: {
                            std::string groupId, keyword;
                            std::cout << "请输入QQ群ID：";
                            std::cin >> groupId;
                            std::cout << "请输入查询关键词：";
                            std::cin >> keyword;
                            BaseGroupLhy* group = nullptr;
                            for (auto g : allGroups) if (g->getGroupId() == groupId) group = g;
                            if (group) group->searchMember(keyword);
                            else std::cout << "群不存在" << std::endl;
                            break;
                        }
                        case 4: {
                            std::string groupId, subGroupName;
                            std::cout << "请输入母群ID：";
                            std::cin >> groupId;
                            std::cout << "请输入临时讨论组名称：";
                            std::cin >> subGroupName;

                            QQGroupLhy* parentGroup = nullptr;
                            auto joinedGroups = qqSvc->getJoinedGroups();
                            for (auto g : joinedGroups) if (g->getGroupId() == groupId) parentGroup = g;

                            if (parentGroup) {
                                QQGroupLhy* subGroup = parentGroup->createTempSubGroup(subGroupName, currentUser);
                                allGroups.push_back(subGroup);
                            }
                            else std::cout << "未加入该群" << std::endl;
                            break;
                        }
                        case 5:
                            goto endQQGroupSubLoop;
                        default:
                            std::cout << "[输入错误] 请输入1-5之间的数字" << std::endl;
                        }
                    }
                endQQGroupSubLoop:
                    break;
                }
                      // QQ-TCP通信
                case 3: {
                    QQServiceLhy* qqSvc = dynamic_cast<QQServiceLhy*>(qqService);
                    if (!qqSvc) {
                        std::cout << "服务异常" << std::endl;
                        break;
                    }

                    std::string ip, message;
                    int port, tcpOp;
                    std::cout << "\n请选择TCP操作：1-发送，2-接收：";
                    std::cin >> tcpOp;

                    switch (tcpOp) {
                    case 1:
                        std::cout << "目标IP：";
                        std::cin >> ip;
                        std::cout << "目标端口：";
                        std::cin >> port;
                        std::cout << "消息内容：";
                        std::cin.ignore();
                        std::getline(std::cin, message);
                        qqSvc->sendTCPMessage(ip, port, message);
                        break;
                    case 2:
                        std::cout << "等待接收消息..." << std::endl;
                        std::cout << qqSvc->receiveTCPMessage() << std::endl;
                        break;
                    default:
                        std::cout << "[输入错误] 请输入1或2" << std::endl;
                    }
                    break;
                }
                      // QQ退出登录
                case 4:
                    qqService->logout();
                    delete qqService;
                    qqService = nullptr;
                    break;
                default:
                    std::cout << "[输入错误] 请输入1-4之间的数字" << std::endl;
                }
            }
            break;
        }
              // 微信服务
        case 2: {
            int wxOp;
            std::cout << "\n===== 微信 服务 =====" << std::endl;
            std::cout << "1. 登录" << std::endl;
            std::cout << "2. 注册" << std::endl;
            std::cout << "请选择操作（1-2）：";
            std::cin >> wxOp;

            BaseUserLhy* wxUser = nullptr;
            BaseServiceLhy* wxService = nullptr;

            if (wxOp == 1) {
                // 微信登录
                std::string userId, code;
                std::cout << "请输入微信ID（测试ID：WX456）：";
                std::cin >> userId;
                std::cout << "请输入验证码（测试验证码：6666）：";
                std::cin >> code;

                wxUser = findUser(allUsers, userId);
                if (!wxUser) {
                    std::cout << "[登录失败] 用户不存在" << std::endl;
                    break;
                }

                wxService = BaseServiceLhy::createService("WECHAT", wxUser);
                if (!wxService || !wxService->login(code)) {
                    delete wxService;
                    break;
                }
                wxService->syncLogin();
            }
            else if (wxOp == 2) {
                // 微信注册
                wxUser = registerWeChatUser();
                allUsers.push_back(wxUser);
                std::cout << "[注册成功] 请登录" << std::endl;

                std::string code = "6666";  // 新用户默认验证码
                wxService = BaseServiceLhy::createService("WECHAT", wxUser);
                wxService->login(code);
                wxService->syncLogin();
            }
            else {
                std::cout << "[输入错误] 请输入1或2" << std::endl;
                break;
            }

            // 微信功能循环
            int wxChoice;
            while (wxService && wxService->getIsLoggedIn()) {
                showWeChatMenu();
                std::cin >> wxChoice;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(1000, '\n');
                    std::cout << "[输入错误] 请输入1-4之间的数字" << std::endl;
                    continue;
                }

                switch (wxChoice) {
                    // 微信好友管理
                case 1: {
                    BaseUserLhy* currentUser = wxService->getUser();
                    std::string friendId;
                    std::cout << "\n当前用户：" << currentUser->getNickname() << "（" << currentUser->getId() << "）" << std::endl;
                    std::cout << "请选择好友操作：1-添加，2-删除，3-查询：";
                    int friendOp;
                    std::cin >> friendOp;

                    switch (friendOp) {
                    case 1:
                        std::cout << "请输入好友ID：";
                        std::cin >> friendId;
                        currentUser->addFriend(friendId) ? std::cout << "添加成功" << std::endl : std::cout << "添加失败" << std::endl;
                        break;
                    case 2:
                        std::cout << "请输入好友ID：";
                        std::cin >> friendId;
                        currentUser->deleteFriend(friendId) ? std::cout << "删除成功" << std::endl : std::cout << "删除失败" << std::endl;
                        break;
                    case 3: {
                        auto friends = currentUser->getFriendIds();
                        std::cout << "好友列表（共" << friends.size() << "人）：" << std::endl;
                        for (const auto& id : friends) std::cout << "  - " << id << std::endl;
                        break;
                    }
                    default:
                        std::cout << "[输入错误] 请输入1-3之间的数字" << std::endl;
                    }
                    break;
                }
                      // 微信群组管理
                case 2: {
                    while (true) {
                        showGroupSubMenu();
                        std::cin >> subChoice;

                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(1000, '\n');
                            std::cout << "[输入错误] 请输入1-5之间的数字" << std::endl;
                            continue;
                        }

                        WeChatServiceLhy* wxSvc = dynamic_cast<WeChatServiceLhy*>(wxService);
                        BaseUserLhy* currentUser = wxService->getUser();

                        switch (subChoice) {
                        case 1: {
                            std::string groupId;
                            std::cout << "请输入微信群ID（测试群：1002）：";
                            std::cin >> groupId;
                            BaseGroupLhy* group = nullptr;
                            for (auto g : allGroups) if (g->getGroupId() == groupId) group = g;
                            if (group && wxSvc) wxSvc->joinGroup(dynamic_cast<WeChatGroupLhy*>(group));
                            else std::cout << "群不存在或服务异常" << std::endl;
                            break;
                        }
                        case 2: {
                            std::string groupId;
                            std::cout << "请输入微信群ID：";
                            std::cin >> groupId;
                            if (wxSvc != nullptr) {
                                // 微信服务存在，执行退出群操作
                                wxSvc->quitGroup(groupId);
                            }
                            else {
                                // 微信服务未初始化，显示异常
                                std::cout << "服务异常" << std::endl;
                            }
                            break;
                        }
                        case 3: {
                            std::string groupId, keyword;
                            std::cout << "请输入微信群ID：";
                            std::cin >> groupId;
                            std::cout << "请输入查询关键词：";
                            std::cin >> keyword;
                            BaseGroupLhy* group = nullptr;
                            for (auto g : allGroups) if (g->getGroupId() == groupId) group = g;
                            if (group) group->searchMember(keyword);
                            else std::cout << "群不存在" << std::endl;
                            break;
                        }
                        case 4: {
                            std::string groupId, announcement;
                            std::cout << "请输入微信群ID：";
                            std::cin >> groupId;
                            std::cout << "请输入群公告：";
                            std::cin.ignore();
                            std::getline(std::cin, announcement);

                            WeChatGroupLhy* wxGroup = nullptr;
                            auto joinedGroups = wxSvc->getJoinedGroups();
                            for (auto g : joinedGroups) if (g->getGroupId() == groupId) wxGroup = g;

                            if (wxGroup) {
                                wxGroup->setAnnouncement(announcement);
                            }
                            else std::cout << "未加入该群" << std::endl;
                            break;
                        }
                        case 5:
                            goto endWeChatGroupSubLoop;
                        default:
                            std::cout << "[输入错误] 请输入1-5之间的数字" << std::endl;
                        }
                    }
                endWeChatGroupSubLoop:
                    break;
                }
                      // 微信-QQ互联
                case 3: {
                    WeChatServiceLhy* wxSvc = dynamic_cast<WeChatServiceLhy*>(wxService);
                    if (!wxSvc) {
                        std::cout << "服务异常" << std::endl;
                        break;
                    }

                    std::string qqId, message;
                    int interOp;
                    std::cout << "\n请选择互联操作：1-发送消息，2-接收消息：";
                    std::cin >> interOp;

                    switch (interOp) {
                    case 1:
                        std::cout << "目标QQ ID：";
                        std::cin >> qqId;
                        std::cout << "消息内容：";
                        std::cin.ignore();
                        std::getline(std::cin, message);
                        wxSvc->sendMessageToQQ(qqId, message);
                        break;
                    case 2:
                        std::cout << "等待接收QQ消息..." << std::endl;
                        std::cout << wxSvc->receiveMessageFromQQ() << std::endl;
                        break;
                    default:
                        std::cout << "[输入错误] 请输入1或2" << std::endl;
                    }
                    break;
                }
                      // 微信退出登录
                case 4:
                    wxService->logout();
                    delete wxService;
                    wxService = nullptr;
                    break;
                default:
                    std::cout << "[输入错误] 请输入1-4之间的数字" << std::endl;
                }
            }
            break;
        }
              // 退出系统
        case 3:
            std::cout << "\n[系统退出] 感谢使用模拟即时通信系统！" << std::endl;
            
            // 保存数据到文件
            UserDataIOLhy::saveUsers(allUsers, USER_FILE);
            UserDataIOLhy::saveGroups(allGroups, GROUP_FILE);
            
            // 清理内存
            for (auto user : allUsers) delete user;
            for (auto group : allGroups) delete group;
            
            return 0;
        default:
            std::cout << "[输入错误] 请输入1-3之间的数字" << std::endl;
        }
    }

    return 0;
}
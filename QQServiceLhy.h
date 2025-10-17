#ifndef QQ_SERVICE_LHY_H
#define QQ_SERVICE_LHY_H

#include "BaseServiceLhy.h"
#include "QQGroupLhy.h"
#include <vector>
#include <iostream>

// �ڰ����κ� Winsock ͷ�ļ�ǰ���� Windows �汾
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#endif
#include <winsock2.h>
#include <ws2tcpip.h>  // ���� InetPtonA ������
#pragma comment(lib, "ws2_32.lib")

// QQ�������֧ࣨ��TCP��Ե�ͨ�š�QQȺ������
class QQServiceLhy : public BaseServiceLhy {
private:
    std::vector<QQGroupLhy*> joinedGroups;  // �û������QQȺ�б�
    SOCKET tcpSocket;                       // TCPͨ��Socket
    bool isTcpInit;                         // TCP��ʼ��״̬

public:
    // ���캯������ʼ��TCPģ��
    QQServiceLhy(BaseUserLhy* user) : BaseServiceLhy(user), isTcpInit(false) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cout << "[QQ����] TCP��ʼ��ʧ�ܣ�Winsock����ʧ��" << std::endl;
            return;
        }
        tcpSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (tcpSocket == INVALID_SOCKET) {
            std::cout << "[QQ����] TCP��ʼ��ʧ�ܣ�Socket����ʧ��" << std::endl;
            WSACleanup();
            return;
        }
        isTcpInit = true;
        std::cout << "[QQ����] TCPͨ��ģ���ʼ���ɹ�" << std::endl;
    }

    ~QQServiceLhy() override {
        // �ͷ�TCP��Դ
        if (isTcpInit) {
            closesocket(tcpSocket);
            WSACleanup();
        }
        // �ͷ�QQȺ�б�
        for (auto group : joinedGroups) {
            delete group;
        }
    }

    // ʵ�ַ������ͽӿ�
    std::string getServiceType() const override {
        return "QQ����";
    }

    // ʵ�ֵ�¼�ӿڣ�QQ��¼������̶�Ϊ"123"��
    bool login(std::string verifyInfo) override {
        if (getIsLoggedIn()) {
            std::cout << "[QQ����] " << user->getNickname() << "�ѵ�¼" << std::endl;
            return true;
        }
        if (verifyInfo != "123") {
            std::cout << "[QQ����] ��¼ʧ�ܣ����������ȷ���룺123��" << std::endl;
            return false;
        }
        setIsLoggedIn(true);
        std::cout << "[QQ����] " << user->getNickname() << "��" << user->getId() << "����¼�ɹ�" << std::endl;
        return true;
    }

    // QQ�������У�����QQȺ
    bool joinGroup(QQGroupLhy* group) {
        if (!getIsLoggedIn()) {
            std::cout << "[QQ����] ���ȵ�¼�ټ���Ⱥ" << std::endl;
            return false;
        }
        if (group->addMember(user)) {
            joinedGroups.push_back(group);
            return true;
        }
        return false;
    }

    // QQ�������У��˳�QQȺ
    bool quitGroup(std::string groupId) {
        for (auto it = joinedGroups.begin(); it != joinedGroups.end(); ++it) {
            if ((*it)->getGroupId() == groupId) {
                (*it)->deleteMember(user->getId());
                joinedGroups.erase(it);
                return true;
            }
        }
        std::cout << "[QQ����] δ����Ⱥ��ID��" << groupId << "��" << std::endl;
        return false;
    }

    // QQ�������У�TCP��Ե�ͨ�ţ�ѡ�����ܣ�
    bool initTCPConnection(std::string ip, int port) {
        if (!isTcpInit || !getIsLoggedIn()) {
            std::cout << "[QQ����] TCP����ʧ�ܣ�δ��ʼ����δ��¼" << std::endl;
            return false;
        }

        sockaddr_in targetAddr;
        ZeroMemory(&targetAddr, sizeof(targetAddr));  // ��ʼ���ṹ��
        targetAddr.sin_family = AF_INET;
        targetAddr.sin_port = htons(port);

        // ʹ��inet_addr����ת��IP��ַ
        targetAddr.sin_addr.s_addr = inet_addr(ip.c_str());
        if (targetAddr.sin_addr.s_addr == INADDR_NONE) {
            std::cout << "[QQ����] ��Ч��IP��ַ��" << ip << "��������룺" << WSAGetLastError() << std::endl;
            return false;
        }

        if (connect(tcpSocket, (SOCKADDR*)&targetAddr, sizeof(targetAddr)) == SOCKET_ERROR) {
            std::cout << "[QQ����] TCP����ʧ�ܣ�Ŀ��IP��" << ip << ":" << port << "�����ɴ������룺" << WSAGetLastError() << std::endl;
            return false;
        }
        std::cout << "[QQ����] TCP���ӳɹ��������ӵ�" << ip << ":" << port << std::endl;
        return true;
    }

    bool sendTCPMessage(std::string ip, int port, std::string message) {
        if (!initTCPConnection(ip, port)) {
            return false;
        }
        message += "\0";  // �����Ϣ����
        int sendLen = send(tcpSocket, message.c_str(), message.size(), 0);
        if (sendLen == SOCKET_ERROR) {
            std::cout << "[QQ����] ��Ϣ����ʧ�ܣ�" << WSAGetLastError() << std::endl;
            return false;
        }
        std::cout << "[QQ����] ��Ϣ���ͳɹ���" << sendLen << "�ֽڣ���" << message << std::endl;
        return true;
    }

    std::string receiveTCPMessage() {
        if (!isTcpInit || !getIsLoggedIn()) {
            return "[����] δ��ʼ����δ��¼";
        }
        char buffer[1024] = { 0 };
        int recvLen = recv(tcpSocket, buffer, sizeof(buffer) - 1, 0);
        if (recvLen <= 0) {
            return "[����] ����ʧ�ܻ����ӶϿ�";
        }
        std::string message(buffer);
        std::cout << "[QQ����] ��Ϣ���ճɹ���" << recvLen << "�ֽڣ���" << message << std::endl;
        return message;
    }

    // Get�ӿڣ���ȡ�û������QQȺ�����ڴ�����ʱ�����飩
    std::vector<QQGroupLhy*> getJoinedGroups() const {
        return joinedGroups;
    }

    QQGroupLhy* createGroup(const std::string& groupId, const std::string& groupName);

};

#endif // QQ_SERVICE_LHY_H
#include "QQServiceLhy.h"

#include <iostream>

QQServiceLhy::QQServiceLhy(BaseUserLhy* user)
    : BaseServiceLhy(user, "QQ" ) {}

bool QQServiceLhy::login(const std::string& credential) {
    if (isLoggedIn()) {
        std::cout << "[QQ] already logged in" << std::endl;
        return true;
    }
    if (!getUser()->verifyPassword(credential)) {
        std::cout << "[QQ] invalid password" << std::endl;
        return false;
    }
    finishLogin();
    std::cout << "[QQ] " << getUser()->getNickname() << " logged in" << std::endl;
    return true;
}

void QQServiceLhy::logout() {
    BaseServiceLhy::logout();
    tcpInbox.clear();
    tcpOutbox.clear();
}

void QQServiceLhy::enqueueOutgoingMessage(const std::string& peerId, const std::string& message) {
    if (!isLoggedIn()) {
        std::cout << "[QQ] please login first" << std::endl;
        return;
    }
    tcpOutbox.emplace_back("To " + peerId + ": " + message);
    std::cout << "[QQ] message queued for " << peerId << std::endl;
}

std::string QQServiceLhy::dequeueIncomingMessage() {
    if (tcpInbox.empty()) {
        return "[QQ] inbox empty";
    }
    std::string msg = tcpInbox.front();
    tcpInbox.pop_front();
    return msg;
}

void QQServiceLhy::emulateIncomingMessage(const std::string& fromId, const std::string& message) {
    tcpInbox.emplace_back("From " + fromId + ": " + message);
}

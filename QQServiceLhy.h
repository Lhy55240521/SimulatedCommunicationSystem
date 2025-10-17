#ifndef QQ_SERVICE_LHY_H
#define QQ_SERVICE_LHY_H

#include "BaseServiceLhy.h"

#include <deque>
#include <string>

class QQGroupLhy;

// QQ service implementation focuses on password login and a lightweight TCP simulation.
class QQServiceLhy : public BaseServiceLhy {
private:
    std::deque<std::string> tcpInbox;
    std::deque<std::string> tcpOutbox;

public:
    explicit QQServiceLhy(BaseUserLhy* user);

    bool login(const std::string& credential) override;
    void logout() override;

    void enqueueOutgoingMessage(const std::string& peerId, const std::string& message);
    std::string dequeueIncomingMessage();
    void emulateIncomingMessage(const std::string& fromId, const std::string& message);
};

#endif // QQ_SERVICE_LHY_H
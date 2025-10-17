#ifndef BASE_SERVICE_LHY_H
#define BASE_SERVICE_LHY_H

#include "BaseUserLhy.h"

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

class BaseServiceLhy {
private:
    BaseUserLhy* user;
    bool loggedIn;
    std::string serviceType;

    static std::unordered_map<std::string, std::vector<BaseServiceLhy*>> userServices;

protected:
    explicit BaseServiceLhy(BaseUserLhy* user, const std::string& serviceType);

    void finishLogin();
    void setLoggedIn(bool value) { loggedIn = value; }

public:
    virtual ~BaseServiceLhy();

    const std::string& getServiceType() const { return serviceType; }
    BaseUserLhy* getUser() const { return user; }
    bool isLoggedIn() const { return loggedIn; }

    virtual bool login(const std::string& credential) = 0;
    virtual void logout();

    static void syncLoginForUser(const std::string& userId, BaseServiceLhy* origin);
    static void registerOpenedServices(BaseUserLhy* user, const std::vector<std::string>& serviceTypes);

    static std::unique_ptr<BaseServiceLhy> create(const std::string& serviceType, BaseUserLhy* user);
};

#endif // BASE_SERVICE_LHY_H
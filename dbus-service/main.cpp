#include <sdbus-c++/sdbus-c++.h>

#include <iostream>
#include <string>

#include "client-creds.h"

#include <pwd.h>
#include <cerrno>
#include <cstring>

#include <systemd/sd-bus.h>

std::vector<std::tuple<std::string, std::string>> objects = {
    {"for all", ""},
    {"only test-user", "testuser"},
};

std::vector<std::string> listObjects(const ClientCredentials& clientCreds)
{
    std::vector<std::string> filteredObjects;
    for (const auto &obj: objects) {

        const std::string objString = std::get<0>(obj);
        const std::string &userName = std::get<1>(obj);

        if (userName.empty()) {
            filteredObjects.push_back(objString);
        } else {
            struct passwd userInfo = {};
            struct passwd *userInfoPtr = nullptr;
            char buf[256];
            int r = getpwnam_r(userName.c_str(), &userInfo, buf, sizeof(buf), &userInfoPtr);
            if (r < 0 || userInfoPtr == nullptr) {
                std::cout << "getpwnam error: " << strerror(errno) << std::endl;
                continue;
            }
            if (userInfo.pw_uid == clientCreds.uid) {
                filteredObjects.push_back(objString);
            }
        }
    }

    return filteredObjects;
}

void listAllObjects(sdbus::MethodCall call)
{
    std::cout << "List All objects: " << std::endl;
    // 1. Get connection credentials and permissions per credentials client. Use Unix user ID
    // 2. While iterating, filter objects according to client permissions
    // Several ways to pass the token to service
    //   * Environment variables/process credentialss
    //   * Permission database (to be queried using process credentials)

    ClientCredentials creds{};
    creds.pid = call.getCredsPid();
    creds.uid = call.getCredsUid();
    creds.gid = call.getCredsGid();
    creds.supplementaryGids = call.getCredsSupplementaryGids();
    creds.linuxSecurityLabel = call.getSELinuxContext();

    auto objects = listObjects(creds);

    auto reply = call.createReply();
    reply << objects;
    reply.send();
}

int main(void)
{
    // Create D-Bus connection to the system bus and requests name on it.
    const char *serviceName = "com.example.ObjectProviderService";
    auto connection = sdbus::createSystemBusConnection(serviceName);

    // Create concatenator D-Bus object.
    const char *objectPath = "/com/example/ObjectProviderService";
    auto objService = sdbus::createObject(*connection, objectPath);

    // Register D-Bus methods and signals on the concatenator object, and exports the object.
    const char *interfaceName = "com.example.ObjectProviderService";

    objService->registerMethod(interfaceName, "ListAllObjects", "", "as", &listAllObjects);
    objService->finishRegistration();

    // Run the I/O event loop on the bus connection.
    connection->enterEventLoop();
    return 0;
}

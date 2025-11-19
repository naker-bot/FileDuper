#ifndef NETWORKSERVICE_CORE_H
#define NETWORKSERVICE_CORE_H

#include <string>

struct NetworkServiceCore {
    std::string ip;
    int port = 0;
    std::string service;     // e.g., "FTP", "NFS"
    std::string type;        // alias for service
    std::string serviceName; // display-friendly name
    std::string status;
    int responseTime = 0; // ms
};

#endif // NETWORKSERVICE_CORE_H

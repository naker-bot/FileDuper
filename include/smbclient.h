#ifndef SMBCLIENT_H
#define SMBCLIENT_H

#include <string>

class SmbClient {
public:
    SmbClient() {}
    ~SmbClient() {}
    
    bool connectToHost(const std::string& host) { return false; }
};

#endif // SMBCLIENT_H

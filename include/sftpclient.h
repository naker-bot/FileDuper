#ifndef SFTPCLIENT_H
#define SFTPCLIENT_H

#include <string>

class SftpClient {
public:
    SftpClient() {}
    ~SftpClient() {}
    
    bool connectToHost(const std::string& host) { return false; }
};

#endif // SFTPCLIENT_H

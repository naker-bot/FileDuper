#ifndef FTPCLIENT_H
#define FTPCLIENT_H

// FTP functionality is fully implemented in main.cpp via libcurl
// This class exists only for linking compatibility
class FtpClient {
public:
    FtpClient();
    ~FtpClient();
};

#endif // FTPCLIENT_H

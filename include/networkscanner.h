#ifndef NETWORKSCANNER_H
#define NETWORKSCANNER_H

#include <string>
#include <atomic>
#include <vector>

class NetworkScanner {
public:
    NetworkScanner();
    ~NetworkScanner();
    
    // Scan single IP with timeout
    bool pingHost(const std::string& ip, int timeout);
    
    // Scan network subnet (e.g., "192.168.1.0/24")
    std::vector<std::string> scanSubnet(const std::string& subnet);
    // Cancelable scan using child process; honors cancel flag (kill child process if set)
    std::vector<std::string> scanSubnetCancelable(const std::string& subnet, std::atomic<bool>* cancel);
    
    // Probe FTP port on host
    bool probeFtpPort(const std::string& ip, int port);

    // Probe TCP port using non-blocking socket with a timeout; optional cancel flag
    // If cancel != nullptr and *cancel becomes true, the probe will abort early.
    bool probePort(const std::string &ip, int port, int timeoutMs, std::atomic<bool> *cancel = nullptr);
};

#endif // NETWORKSCANNER_H

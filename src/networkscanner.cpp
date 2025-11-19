#include "networkscanner.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <thread>
#include <vector>
#include <algorithm>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <poll.h>
#include <unistd.h>
#include <errno.h>

NetworkScanner::NetworkScanner() {
    std::cout << "[NetworkScanner] Initialized with multi-threaded scanning" << std::endl;
}

NetworkScanner::~NetworkScanner() {
}

// Scan single IP with timeout
bool NetworkScanner::pingHost(const std::string& ip, int timeout) {
    std::string cmd = "ping -c 1 -W " + std::to_string(timeout) + " " + ip + " > /dev/null 2>&1";
    int result = system(cmd.c_str());
    return (result == 0);
}

// Scan network subnet (e.g., "192.168.1.0/24")
std::vector<std::string> NetworkScanner::scanSubnet(const std::string& subnet) {
    std::cout << "[NetworkScanner] Scanning subnet: " << subnet << std::endl;
    
    std::vector<std::string> hosts;
    
    // Try nmap first (fast)
    std::string nmapCmd = "timeout 10 nmap -sn " + subnet + 
                         " 2>/dev/null | grep 'Nmap scan report' | awk '{print $5}' > /tmp/fileduper_scan.txt";
    system(nmapCmd.c_str());
    
    std::ifstream scanFile("/tmp/fileduper_scan.txt");
    if (scanFile.is_open()) {
        std::string line;
        while (std::getline(scanFile, line)) {
            if (!line.empty() && line != "()" && line.find('.') != std::string::npos) {
                hosts.push_back(line);
                std::cout << "[NetworkScanner]   Found: " << line << std::endl;
            }
        }
        scanFile.close();
    }
    
    // Fallback to ARP if nmap found nothing
    if (hosts.empty()) {
        std::cout << "[NetworkScanner] Using ARP fallback..." << std::endl;
        system("arp -a | grep -oE '([0-9]{1,3}\\.){3}[0-9]{1,3}' > /tmp/fileduper_arp.txt");
        
        std::ifstream arpFile("/tmp/fileduper_arp.txt");
        if (arpFile.is_open()) {
            std::string line;
            while (std::getline(arpFile, line)) {
                if (!line.empty()) {
                    hosts.push_back(line);
                    std::cout << "[NetworkScanner]   Found (ARP): " << line << std::endl;
                }
            }
            arpFile.close();
        }
    }
    
    // Remove duplicates and sort
    std::sort(hosts.begin(), hosts.end());
    hosts.erase(std::unique(hosts.begin(), hosts.end()), hosts.end());
    
    std::cout << "[NetworkScanner] Scan complete: " << hosts.size() << " hosts found" << std::endl;
    return hosts;
}

std::vector<std::string> NetworkScanner::scanSubnetCancelable(const std::string& subnet, std::atomic<bool>* cancel) {
    std::cout << "[NetworkScanner] Cancelable Scanning subnet: " << subnet << std::endl;

    std::vector<std::string> hosts;

    // Use fork/exec so we can kill the child process if cancel is requested
    int pipefd[2];
    if (pipe(pipefd) != 0) {
        return hosts;
    }

    pid_t pid = fork();
    if (pid == 0) {
        // child
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[0]);
        close(pipefd[1]);
        std::string cmd = "nmap -sn " + subnet + " 2>/dev/null | grep 'Nmap scan report' | awk '{print $5}'";
        execl("/bin/sh", "sh", "-c", cmd.c_str(), (char*)NULL);
        _exit(127);
    }

    // parent
    close(pipefd[1]);

    FILE *f = fdopen(pipefd[0], "r");
    if (!f) {
        close(pipefd[0]);
        return hosts;
    }

    char buf[512];
    while (fgets(buf, sizeof(buf), f)) {
        if (cancel && cancel->load()) {
            // kill child
            kill(pid, SIGTERM);
            break;
        }
        std::string line(buf);
        // strip newline
        while (!line.empty() && (line.back() == '\n' || line.back() == '\r')) line.pop_back();
        if (!line.empty() && line.find('.') != std::string::npos) {
            hosts.push_back(line);
            std::cout << "[NetworkScanner]   Found: " << line << std::endl;
        }
    }

    fclose(f);

    // wait for child
    int status = 0;
    waitpid(pid, &status, 0);

    // Fallback to ARP if nothing and not canceled
    if (!cancel || !cancel->load()) {
        if (hosts.empty()) {
            std::cout << "[NetworkScanner] Using ARP fallback..." << std::endl;
            system("arp -a | grep -oE '([0-9]{1,3}\\.){3}[0-9]{1,3}' > /tmp/fileduper_arp.txt");
            std::ifstream arpFile("/tmp/fileduper_arp.txt");
            if (arpFile.is_open()) {
                std::string line;
                while (std::getline(arpFile, line)) {
                    if (!line.empty()) {
                        hosts.push_back(line);
                        std::cout << "[NetworkScanner]   Found (ARP): " << line << std::endl;
                    }
                }
                arpFile.close();
            }
        }
    }

    std::sort(hosts.begin(), hosts.end());
    hosts.erase(std::unique(hosts.begin(), hosts.end()), hosts.end());
    std::cout << "[NetworkScanner] Cancelable scan complete: " << hosts.size() << " hosts found" << std::endl;
    return hosts;
}

// Probe FTP port on host
bool NetworkScanner::probeFtpPort(const std::string& ip, int port) {
    std::string cmd = "timeout 2 nc -z -w 1 " + ip + " " + std::to_string(port) + " 2>/dev/null";
    int result = system(cmd.c_str());
    return (result == 0);
}

bool NetworkScanner::probePort(const std::string &ip, int port, int timeoutMs, std::atomic<bool> *cancel) {
    // Create a non-blocking socket and try to connect; this allows cancellation and a timeout
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    // Set non-blocking
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    if (inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0) {
        close(sock);
        return false;
    }

    int rc = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (rc == 0) {
        close(sock);
        return true; // immediate connect
    }

    if (errno != EINPROGRESS) {
        close(sock);
        return false;
    }

    struct pollfd pfd;
    pfd.fd = sock;
    pfd.events = POLLOUT;

    int elapsed = 0;
    const int step = 50; // ms
    while (elapsed < timeoutMs) {
        if (cancel && cancel->load()) {
            close(sock);
            return false;
        }
        int ret = poll(&pfd, 1, step);
        if (ret > 0) {
            // Check if connected
            int err = 0;
            socklen_t len = sizeof(err);
            if (getsockopt(sock, SOL_SOCKET, SO_ERROR, &err, &len) == 0 && err == 0) {
                close(sock);
                return true;
            }
            break;
        }
        elapsed += step;
    }

    close(sock);
    return false;
}

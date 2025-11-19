// Windows-specific network scanner
#include "networkscanner.h"
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <atomic>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

// Link with Ws2_32.lib on MSVC; CMake will link properly on Windows via find_package or target libs

NetworkScanner::NetworkScanner() {
    std::cout << "[NetworkScanner] Windows: Initialized" << std::endl;
}

NetworkScanner::~NetworkScanner() {
}

bool NetworkScanner::pingHost(const std::string& ip, int timeout) {
    // Windows ping: -n count, -w timeout (ms)
    std::string cmd = "ping -n 1 -w " + std::to_string(timeout) + " " + ip + " > nul 2>&1";
    int result = system(cmd.c_str());
    return (result == 0);
}

std::vector<std::string> NetworkScanner::scanSubnet(const std::string& subnet) {
    std::cout << "[NetworkScanner] Windows: Scanning subnet: " << subnet << std::endl;
    std::vector<std::string> hosts;

    // try nmap if available
    std::string nmapCmd = "nmap -sn " + subnet + " 2>nul | findstr /C:\"Nmap scan report for\" > C:\\Windows\\Temp\\fileduper_scan.txt";
    system(nmapCmd.c_str());
    std::ifstream scanFile("C:\\Windows\\Temp\\fileduper_scan.txt");
    if (scanFile.is_open()) {
        std::string line;
        while (std::getline(scanFile, line)) {
            // on Windows, the line may contain 'Nmap scan report for <ip>' - take the last token
            std::istringstream iss(line);
            std::string token;
            std::vector<std::string> tokens;
            while (iss >> token) tokens.push_back(token);
            if (!tokens.empty()) {
                std::string ip = tokens.back();
                if (!ip.empty() && ip.find('.') != std::string::npos) {
                    hosts.push_back(ip);
                    std::cout << "[NetworkScanner]   Found: " << ip << std::endl;
                }
            }
        }
        scanFile.close();
    }

    if (hosts.empty()) {
        std::cout << "[NetworkScanner] Windows: Using ARP fallback..." << std::endl;
        system("arp -a > C:\\Windows\\Temp\\fileduper_arp.txt");
        std::ifstream arpFile("C:\\Windows\\Temp\\fileduper_arp.txt");
        if (arpFile.is_open()) {
            std::string line;
            while (std::getline(arpFile, line)) {
                // parse IPv4 address
                std::string ip;
                for (size_t i = 0; i < line.size(); ++i) {
                    if ((isdigit(line[i]) || line[i] == '.') && (ip.empty() || ip.back() != ' ')) ip += line[i];
                    else if (!ip.empty() && ip.back() != ' ') ip += ' ';
                }
                std::istringstream iss(line);
                std::string token;
                while (iss >> token) {
                    if (token.find('.') != std::string::npos) {
                        hosts.push_back(token);
                        std::cout << "[NetworkScanner]   Found (ARP): " << token << std::endl;
                    }
                }
            }
            arpFile.close();
        }
    }

    std::sort(hosts.begin(), hosts.end());
    hosts.erase(std::unique(hosts.begin(), hosts.end()), hosts.end());

    std::cout << "[NetworkScanner] Windows: Scan complete: " << hosts.size() << " hosts found" << std::endl;
    return hosts;
}

std::vector<std::string> NetworkScanner::scanSubnetCancelable(const std::string& subnet, std::atomic<bool>* cancel) {
    std::cout << "[NetworkScanner] Windows: Cancelable Scanning subnet: " << subnet << std::endl;
    std::vector<std::string> hosts;

    // Create a child process for nmap and capture stdout; can kill via TerminateProcess if cancel is set
    // CreatePipe and CreateProcess
    HANDLE readPipe = NULL;
    HANDLE writePipe = NULL;
    SECURITY_ATTRIBUTES saAttr;
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;
    if (!CreatePipe(&readPipe, &writePipe, &saAttr, 0)) {
        return hosts;
    }

    // Make sure the read handle is not inherited.
    SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0);

    std::string cmd = "nmap -sn " + subnet + " 2>nul";
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESTDHANDLES;
    si.hStdOutput = writePipe;
    si.hStdError = writePipe;
    si.hStdInput = NULL;

    if (!CreateProcessA(NULL, (LPSTR)cmd.c_str(), NULL, NULL, TRUE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi)) {
        CloseHandle(readPipe);
        CloseHandle(writePipe);
        return hosts;
    }

    CloseHandle(writePipe);

    // Read from readPipe line by line
    char buf[512];
    DWORD readBytes = 0;
    std::string acc;
    while (true) {
        if (cancel && cancel->load()) {
            TerminateProcess(pi.hProcess, 1);
            break;
        }
        BOOL ok = ReadFile(readPipe, buf, sizeof(buf)-1, &readBytes, NULL);
        if (!ok || readBytes == 0) break;
        buf[readBytes] = '\0';
        acc += buf;
        size_t pos;
        while ((pos = acc.find('\n')) != std::string::npos) {
            std::string line = acc.substr(0, pos);
            acc.erase(0, pos+1);
            if (line.find("Nmap scan report for") != std::string::npos) {
                // extract last token
                std::istringstream iss(line);
                std::string token;
                std::vector<std::string> tokens;
                while (iss >> token) tokens.push_back(token);
                if (!tokens.empty()) {
                    std::string ip = tokens.back();
                    if (!ip.empty() && ip.find('.') != std::string::npos) {
                        hosts.push_back(ip);
                        std::cout << "[NetworkScanner]   Found: " << ip << std::endl;
                    }
                }
            }
        }
        // brief sleep to avoid CPU spin
        Sleep(10);
    }

    // Wait for child process to exit
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    CloseHandle(readPipe);

    if (!cancel || !cancel->load()) {
        if (hosts.empty()) {
            std::cout << "[NetworkScanner] Windows: Using ARP fallback..." << std::endl;
            system("arp -a > C:\\Windows\\Temp\\fileduper_arp.txt");
            std::ifstream arpFile("C:\\Windows\\Temp\\fileduper_arp.txt");
            if (arpFile.is_open()) {
                std::string line;
                while (std::getline(arpFile, line)) {
                    std::istringstream iss(line);
                    std::string token;
                    while (iss >> token)
                        if (token.find('.') != std::string::npos) {
                            hosts.push_back(token);
                            std::cout << "[NetworkScanner]   Found (ARP): " << token << std::endl;
                        }
                }
                arpFile.close();
            }
        }
    }

    std::sort(hosts.begin(), hosts.end());
    hosts.erase(std::unique(hosts.begin(), hosts.end()), hosts.end());

    std::cout << "[NetworkScanner] Windows: Cancelable scan complete: " << hosts.size() << " hosts found" << std::endl;
    return hosts;
}

// Probe FTP port using probePort implementation
bool NetworkScanner::probeFtpPort(const std::string& ip, int port) {
    return probePort(ip, port, 1000, nullptr);
}

bool NetworkScanner::probePort(const std::string &ip, int port, int timeoutMs, std::atomic<bool> *cancel) {
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        return false;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        WSACleanup();
        return false;
    }

    u_long mode = 1; // non-blocking
    ioctlsocket(sock, FIONBIO, &mode);

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons((u_short)port);
    inet_pton(AF_INET, ip.c_str(), &addr.sin_addr);

    int rc = connect(sock, (struct sockaddr*)&addr, sizeof(addr));
    if (rc == 0) {
        closesocket(sock);
        WSACleanup();
        return true;
    }

    int lastErr = WSAGetLastError();
    if (lastErr != WSAEWOULDBLOCK && lastErr != WSAEINPROGRESS) {
        closesocket(sock);
        WSACleanup();
        return false;
    }

    // Wait with select
    FD_SET writeSet;
    FD_ZERO(&writeSet);
    FD_SET(sock, &writeSet);

    int elapsed = 0;
    const int step = 50; // ms
    while (elapsed < timeoutMs) {
        if (cancel && cancel->load()) {
            closesocket(sock);
            WSACleanup();
            return false;
        }
        timeval tv;
        tv.tv_sec = 0;
        tv.tv_usec = step * 1000;
        FD_SET(sock, &writeSet);
        int ret = select(0, NULL, &writeSet, NULL, &tv);
        if (ret > 0) {
            int err = 0;
            int len = sizeof(err);
            if (getsockopt(sock, SOL_SOCKET, SO_ERROR, (char*)&err, &len) == 0 && err == 0) {
                closesocket(sock);
                WSACleanup();
                return true;
            }
            break;
        }
        elapsed += step;
    }

    closesocket(sock);
    WSACleanup();
    return false;
}

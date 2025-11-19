#include "net_utils.h"
#include <arpa/inet.h>
#include <sstream>
#include <vector>
#include <future>
#include <mutex>
#include <ifaddrs.h>
#include <net/if.h>

bool cidrToRange(const std::string& cidr, uint32_t &outStart, uint32_t &outEnd) {
    size_t slash = cidr.find('/');
    std::string ipStr = (slash == std::string::npos) ? cidr : cidr.substr(0, slash);
    int prefix = (slash == std::string::npos) ? 32 : std::stoi(cidr.substr(slash+1));
    if (prefix < 0 || prefix > 32) return false;

    // Allow shorter IPs like "10.0/16" -> expand to "10.0.0.0"
    auto expandPartialIp = [](const std::string &ip) -> std::string {
        std::vector<std::string> parts;
        std::istringstream iss(ip);
        std::string tok;
        while (std::getline(iss, tok, '.')) {
            if (!tok.empty()) parts.push_back(tok);
        }
        if (parts.empty()) return std::string();
        // Validate numbers
        for (const auto &p : parts) {
            for (char c : p) if (!isdigit((unsigned char)c)) return std::string();
            if (std::stoi(p) < 0 || std::stoi(p) > 255) return std::string();
        }
        while (parts.size() < 4) parts.push_back("0");
        return parts[0] + "." + parts[1] + "." + parts[2] + "." + parts[3];
    };

    std::string expanded = expandPartialIp(ipStr);
    if (expanded.empty()) return false;
    struct in_addr addr;
    if (inet_pton(AF_INET, expanded.c_str(), &addr) != 1) return false;
    uint32_t ip = ntohl(addr.s_addr);

    uint64_t mask = (prefix == 0) ? 0 : (~0ULL << (32 - prefix)) & 0xFFFFFFFFULL;
    uint32_t net = ip & (uint32_t)mask;
    uint32_t broadcast = (uint32_t)(net | (~(uint32_t)mask));

    if (prefix == 32) {
        outStart = outEnd = net;
    } else if (prefix == 31) {
        outStart = net; outEnd = broadcast;
    } else {
        outStart = net + 1;
        outEnd = broadcast - 1;
    }
    return true;
}

std::string normalizeSubnet(const std::string& subnet) {
    std::string normalized = subnet;
    if (normalized.find('/') == std::string::npos) {
        std::vector<std::string> parts;
        std::istringstream iss(normalized);
        std::string tok;
        while (std::getline(iss, tok, '.')) {
            if (!tok.empty()) parts.push_back(tok);
        }
        if (parts.size() == 1) normalized = parts[0] + ".0.0.0/8";
        else if (parts.size() == 2) normalized = parts[0] + "." + parts[1] + ".0.0/16";
        else if (parts.size() == 3) normalized = parts[0] + "." + parts[1] + "." + parts[2] + ".0/24";
    }
    // If user provides a partial ip with slash like "10.0/16", expand IP portion
    size_t slash = normalized.find('/');
    if (slash != std::string::npos) {
        std::string ipPart = normalized.substr(0, slash);
        // expand short ip tokens to full dotted quad
        std::vector<std::string> parts;
        std::istringstream iss(ipPart);
        std::string tok;
        while (std::getline(iss, tok, '.')) { if (!tok.empty()) parts.push_back(tok); }
        if (parts.size() > 0 && parts.size() < 4) {
            while (parts.size() < 4) parts.push_back("0");
            std::string expandedIp = parts[0] + "." + parts[1] + "." + parts[2] + "." + parts[3];
            normalized = expandedIp + normalized.substr(slash);
        }
    }
    return normalized;
}

// Ping a single host via system 'ping -c 1 -W <sec>' (returns true if reachable)
bool pingHost(const std::string &ip, int timeoutSeconds) {
    std::string cmd = "ping -c 1 -W " + std::to_string(timeoutSeconds) + " -n " + ip + " >/dev/null 2>&1";
    int res = system(cmd.c_str());
    return (res == 0);
}

void pingHostRange(const std::string &startIpStr, const std::string &endIpStr, int timeoutSeconds, std::vector<std::string> *results) {
    auto ip2int = [](const std::string &ip) -> uint32_t {
        struct in_addr addr;
        if (inet_pton(AF_INET, ip.c_str(), &addr) != 1) return 0;
        return ntohl(addr.s_addr);
    };
    auto int2ip = [](uint32_t v) -> std::string {
        struct in_addr s; s.s_addr = htonl(v);
        char buf[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &s, buf, sizeof(buf));
        return std::string(buf);
    };

    uint32_t start = ip2int(startIpStr);
    uint32_t end = ip2int(endIpStr);
    if (start == 0 || end == 0 || end < start) return;

    const int BATCH_SIZE = 100;
    // Local mutex for protecting results vector within this function
    std::mutex resultsMutex;
    std::vector<std::future<bool>> futures;
    uint32_t ipCounter = start;
    for (uint32_t ip = start; ip <= end; ++ip) {
        std::string ipStr = int2ip(ip);
        futures.push_back(std::async(std::launch::async, [ipStr, timeoutSeconds]() {
            return pingHost(ipStr, timeoutSeconds);
        }));

        if ((int)futures.size() >= BATCH_SIZE || ip == end) {
            uint32_t cur = ip - (futures.size()-1);
            for (auto &f : futures) {
                bool ok = f.get();
                if (ok) {
                        std::lock_guard<std::mutex> lock(resultsMutex);
                        results->push_back(int2ip(cur));
                    }
                cur++;
            }
            futures.clear();
        }
    }
}

// Determine whether 'cidr' overlaps any local interface.
bool isSubnetLocal(const std::string &cidr) {
    uint32_t start=0, end=0;
    if (!cidrToRange(cidr, start, end)) return false;

    struct ifaddrs *ifaddr = nullptr;
    if (getifaddrs(&ifaddr) == -1) return false;

    for (struct ifaddrs *ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (!ifa->ifa_addr) continue;
        if (ifa->ifa_addr->sa_family != AF_INET) continue;
        struct sockaddr_in *sin = (struct sockaddr_in*)ifa->ifa_addr;
        struct sockaddr_in *mask = (struct sockaddr_in*)ifa->ifa_netmask;
        uint32_t addr = ntohl(sin->sin_addr.s_addr);
        uint32_t m = ntohl(mask->sin_addr.s_addr);
        uint32_t localStart = addr & m;
        uint32_t localEnd = localStart | (~m & 0xFFFFFFFF);

        // treat loopback as local as well

        // Overlap check
        if (!(end < localStart || start > localEnd)) {
            freeifaddrs(ifaddr);
            return true;
        }
    }

    freeifaddrs(ifaddr);
    return false;
}

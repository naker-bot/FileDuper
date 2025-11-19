#include "net_utils.h"
#include <arpa/inet.h>
#include <sstream>
#include <vector>

bool cidrToRange(const std::string& cidr, uint32_t &outStart, uint32_t &outEnd) {
    size_t slash = cidr.find('/');
    std::string ipStr = (slash == std::string::npos) ? cidr : cidr.substr(0, slash);
    int prefix = (slash == std::string::npos) ? 32 : std::stoi(cidr.substr(slash+1));
    if (prefix < 0 || prefix > 32) return false;

    struct in_addr addr;
    if (inet_pton(AF_INET, ipStr.c_str(), &addr) != 1) return false;
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
        if (parts.size() == 2) normalized = parts[0] + "." + parts[1] + ".0.0/24";
        else if (parts.size() == 3) normalized = parts[0] + "." + parts[1] + "." + parts[2] + ".0/24";
    }
    return normalized;
}

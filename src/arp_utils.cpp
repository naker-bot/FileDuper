#include "arp_utils.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::string> getActiveHostsViaARP() {
    std::vector<std::string> activeHosts;
    std::ifstream arpFile("/proc/net/arp");
    if (!arpFile.is_open()) return activeHosts;

    std::string line;
    std::getline(arpFile, line); // skip header
    while (std::getline(arpFile, line)) {
        std::istringstream iss(line);
        std::string ip, hw, flags, mac, mask, device;
        if (iss >> ip >> hw >> flags >> mac >> mask >> device) {
            int flagVal = std::stoi(flags, nullptr, 16);
            if ((flagVal & 0x2) || (flagVal & 0x6)) {
                activeHosts.push_back(ip);
            }
        }
    }
    return activeHosts;
}

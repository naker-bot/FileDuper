#include <iostream>
#include "arp_utils.h"

int main() {
    auto hosts = getActiveHostsViaARP();
    std::cout << "ARP hosts found: " << hosts.size() << std::endl;
    for (const auto &h : hosts) std::cout << " - " << h << '\n';
    return 0;
}

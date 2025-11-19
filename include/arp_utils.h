#pragma once
#include <vector>
#include <string>

// Returns active hosts discovered via ARP (/proc/net/arp) on Linux.
std::vector<std::string> getActiveHostsViaARP();

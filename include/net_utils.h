#pragma once
#include <string>
#include <cstdint>
#include <vector>

// Convert CIDR to numeric start and end IPs (inclusive host range)
bool cidrToRange(const std::string& cidr, uint32_t &outStart, uint32_t &outEnd);

// Normalize shorthand notations like "10.0" -> "10.0.0.0/24"
std::string normalizeSubnet(const std::string& subnet);

// Ping helpers
bool pingHost(const std::string &ip, int timeoutSeconds);
void pingHostRange(const std::string &startIpStr, const std::string &endIpStr, int timeoutSeconds, std::vector<std::string> *results);

// Returns true if the provided subnet (CIDR) overlaps any locally configured
// network interface. Used to select ARP-based discovery for local networks.
bool isSubnetLocal(const std::string &cidr);

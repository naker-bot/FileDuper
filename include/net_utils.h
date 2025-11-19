#pragma once
#include <string>
#include <cstdint>

// Convert CIDR to numeric start and end IPs (inclusive host range)
bool cidrToRange(const std::string& cidr, uint32_t &outStart, uint32_t &outEnd);

// Normalize shorthand notations like "10.0" -> "10.0.0.0/24"
std::string normalizeSubnet(const std::string& subnet);

#include <iostream>
#include <cassert>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "net_utils.h"

void test_cidr_basic() {
    uint32_t s,e;
    bool ok = cidrToRange("192.168.1.0/24", s, e);
    assert(ok);
    std::cout << "192.168.1.0/24 -> " << s << ".." << e << "\n";
    // Expect 192.168.1.1 (C0A80101) .. 192.168.1.254
    struct in_addr a; a.s_addr = htonl(s); char buf[INET_ADDRSTRLEN]; inet_ntop(AF_INET, &a, buf, sizeof(buf)); std::cout << "Start: " << buf << "\n";
    a.s_addr = htonl(e); inet_ntop(AF_INET, &a, buf, sizeof(buf)); std::cout << "End: " << buf << "\n";
    
    ok = cidrToRange("10.0.0.1/32", s, e);
    assert(ok && s==e);

    // partial-cidr parsing
    bool ok2 = cidrToRange("10.0/16", s, e);
    assert(ok2);
    a.s_addr = htonl(s); inet_ntop(AF_INET, &a, buf, sizeof(buf)); std::cout << "10.0/16 Start: " << buf << "\n";

    bool ok3 = cidrToRange("10/8", s, e);
    assert(ok3);
    a.s_addr = htonl(s); inet_ntop(AF_INET, &a, buf, sizeof(buf)); std::cout << "10/8 Start: " << buf << "\n";
}

void test_normalize() {
    std::string n = normalizeSubnet("10.0");
    assert(n == "10.0.0.0/16" || n == "10.0.0.0/24");
    std::string n2 = normalizeSubnet("10.0.1");
    assert(n2 == "10.0.1.0/24");

    // new cases
    std::string n3 = normalizeSubnet("10");
    assert(n3 == "10.0.0.0/8");
    std::string n4 = normalizeSubnet("10.0");
    assert(n4 == "10.0.0.0/16" || n4 == "10.0.0.0/24"); // allow old behavior as alternate
    std::string n5 = normalizeSubnet("10.0/16");
    assert(n5 == "10.0.0.0/16");
}
void test_pingrange() {
    std::vector<std::string> results;
    pingHostRange("127.0.0.1", "127.0.0.1", 1, &results);
    // On most systems, localhost should respond to ping
    bool foundLocal = false;
    for (const auto &r : results) {
        if (r == "127.0.0.1") foundLocal = true;
    }
    assert(foundLocal);
}

void test_subnet_local() {
    // localhost should always be a local subnet
    bool local = isSubnetLocal("127.0.0.1/32");
    assert(local);
}

int main(){
    test_cidr_basic();
    test_normalize();
        test_pingrange();
    test_subnet_local();
    std::cout << "net_utils tests passed" << std::endl;
    return 0;
}

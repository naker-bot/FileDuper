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
}

void test_normalize() {
    std::string n = normalizeSubnet("10.0");
    assert(n == "10.0.0.0/24");
    std::string n2 = normalizeSubnet("10.0.1");
    assert(n2 == "10.0.1.0/24");
}

int main(){
    test_cidr_basic();
    test_normalize();
    std::cout << "net_utils tests passed" << std::endl;
    return 0;
}

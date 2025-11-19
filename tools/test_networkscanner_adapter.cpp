#include <iostream>
#include <future>
#include "networkscanner_adapter.h"

int main(int argc, char **argv) {
    NetworkScannerAdapter adapter;

    std::promise<void> done;
    auto done_f = done.get_future();

    adapter.setServiceFoundCallback([&](const NetworkServiceCore &s){
        std::cout << "Service found: " << s.ip << ":" << s.port << " (" << s.service << ")\n";
    });
    adapter.setScanFinishedCallback([&](){
        done.set_value();
    });

    adapter.setIpRange("192.168.1.0/24");
    adapter.startScan();

    // Wait for scan to complete
    done_f.wait();
    std::cout << "Scan finished." << std::endl;
    return 0;
}

#include <iostream>
#include <thread>
#include <chrono>
#include <future>
#include "networkscanner_adapter.h"

int main() {
    NetworkScannerAdapter adapter;
    std::promise<void> done;
    auto f = done.get_future();

    // Record timestamps of callbacks for strict cancel check
    std::mutex mtx;
    std::vector<std::chrono::steady_clock::time_point> cbTimes;

    adapter.setServiceFoundCallback([&](const NetworkServiceCore &s){
        std::lock_guard<std::mutex> lock(mtx);
        cbTimes.push_back(std::chrono::steady_clock::now());
        std::cout << "Service found while scanning: " << s.ip << ":" << s.port << "\n";
    });
    adapter.setScanFinishedCallback([&](){
        std::cout << "Adapter finished.\n";
        done.set_value();
    });

    adapter.setIpRange("192.168.1.0/24");
    adapter.startScan();

    // Wait a short while then cancel
    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    auto cancelTime = std::chrono::steady_clock::now();
    std::cout << "Cancelling scan..." << std::endl;
    adapter.stopScan();

    // Wait for the adapter to signal finished to be safe
    f.wait();
    std::cout << "Cancel test done." << std::endl;
    
    // Strict verification: No callbacks should be recorded after cancelTime
    {
        std::lock_guard<std::mutex> lock(mtx);
        for (const auto &t : cbTimes) {
            if (t > cancelTime) {
                std::cerr << "❌ Error: Callback occurred after stopScan()\n";
                return 2;
            }
        }
    }

    std::cout << "✅ No callbacks after stopScan() verified." << std::endl;
    return 0;
}

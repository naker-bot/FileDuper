#ifndef NETWORKSCANNER_ADAPTER_H
#define NETWORKSCANNER_ADAPTER_H

#include <functional>
#include <string>
#include <atomic>
#include <thread>
#include "networkservice_core.h"

// Forward declaration of the non-Qt scanner
class NetworkScanner;

class NetworkScannerAdapter {
public:
    NetworkScannerAdapter();
    ~NetworkScannerAdapter();

    // Control
    void setIpRange(const std::string &range);
    void startScan();
    void stopScan();

    // Callback setters - GUI will set these to convert to signals
    void setServiceFoundCallback(std::function<void(const NetworkServiceCore &)> cb) { m_serviceFoundCb = std::move(cb); }
        void setScanFinishedCallback(std::function<void()> cb) { m_scanFinishedCb = std::move(cb); }
        // Progress callback: (processedCount, totalCount)
        void setProgressCallback(std::function<void(int,int)> cb) { m_progressCb = std::move(cb); }

private:
    NetworkScanner *m_backend = nullptr;
    std::function<void(const NetworkServiceCore &)> m_serviceFoundCb;
        std::function<void()> m_scanFinishedCb;
        std::function<void(int,int)> m_progressCb;
    std::atomic<bool> m_stop{false};
    // track worker thread to allow graceful stop and join
    std::thread *m_worker = nullptr;
    std::string m_ipRange;
};

#endif // NETWORKSCANNER_ADAPTER_H

#include "networkscanner_adapter.h"
#include "networkscanner.h"
#include <thread>
#include <chrono>

NetworkScannerAdapter::NetworkScannerAdapter() {
    m_backend = new NetworkScanner();
}

NetworkScannerAdapter::~NetworkScannerAdapter() {
    m_stop = true;
    if (m_worker) {
        if (m_worker->joinable()) {
            m_worker->join();
        }
        delete m_worker;
        m_worker = nullptr;
    }
    delete m_backend;
    m_backend = nullptr;
}

void NetworkScannerAdapter::setIpRange(const std::string &range) {
    // pass-through for now; non-Qt scanner uses string-based API
    // store range or ignore for future use
    m_ipRange = range;
}

void NetworkScannerAdapter::startScan() {
    if (!m_backend) return;
    m_stop = false;

    // spawn worker thread and retain pointer so we can join on stop
    m_worker = new std::thread([this]() {
        auto hosts = m_backend->scanSubnetCancelable(m_ipRange.empty() ? "192.168.1.0/24" : m_ipRange, &m_stop);
        int total = (int)hosts.size();
        for (int i = 0; i < total; ++i) {
            const std::string &ip = hosts[i];
            if (m_stop) break;
            if (m_progressCb) m_progressCb(i + 1, total);
            // Use the non-blocking probe to allow immediate cancellation
            if (m_backend->probePort(ip, 21, 2000, &m_stop)) {
                NetworkServiceCore s;
                s.ip = ip;
                s.port = 21;
                s.service = "FTP";
                s.serviceName = "FTP";
                s.status = "Online";
                if (m_serviceFoundCb) m_serviceFoundCb(s);
            }
            // small sleep to avoid hammering network
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }

        if (m_scanFinishedCb) m_scanFinishedCb();
    });
}

void NetworkScannerAdapter::stopScan() {
    m_stop = true;
    if (m_worker) {
        if (m_worker->joinable()) {
            m_worker->join();
        }
        delete m_worker;
        m_worker = nullptr;
    }
}

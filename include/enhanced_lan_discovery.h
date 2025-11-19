#ifndef ENHANCED_LAN_DISCOVERY_H
#define ENHANCED_LAN_DISCOVERY_H

#include <QObject>
#include <QHostAddress>

class QUdpSocket;
class QTcpSocket;
class QTimer;

class EnhancedLANDiscovery : public QObject
{
    Q_OBJECT
    
public:
    struct SubnetRange {
        QString baseIP;
        int startSubnet;
        int endSubnet;
        quint16 port;
    };
    
    explicit EnhancedLANDiscovery(QObject *parent = nullptr);
    
    void startMultiSubnetScan();
    void stopMultiSubnetScan();
    void addSubnetRange(const QString &baseIP, int startSubnet, int endSubnet, quint16 port);
    void enableAutoConnect(bool enabled);
    void setAutoConnectDelay(int seconds);
    
signals:
    void peerDiscovered(const QString &peerName, const QHostAddress &ip, quint16 port);
    void peerConnected(const QString &peerName, const QHostAddress &ip, quint16 port);
    void scanProgress(int currentSubnet, int totalSubnets);
    void scanCompleted(int peersFound);
    
private slots:
    void processNextSubnet();
    void onDiscoveryResponse();
    void attemptAutoConnect();
    
private:
    QList<SubnetRange> m_subnetRanges;
    int m_currentRangeIndex;
    int m_currentSubnet;
    int m_currentHost;
    QTimer *m_scanTimer;
    QUdpSocket *m_discoverySocket;
    bool m_autoConnect;
    int m_autoConnectDelay;
    QTimer *m_autoConnectTimer;
    QList<QPair<QHostAddress, quint16>> m_discoveredPeers;
    
    static const int HOSTS_PER_SUBNET = 254;
    static const int SCAN_TIMEOUT_MS = 50;
    
    void sendDiscoveryPacket(const QHostAddress &target, quint16 port);
    void processDiscoveryResponse(const QByteArray &data, const QHostAddress &sender, quint16 port);
    bool isValidEnigmaChatPeer(const QJsonObject &response);
};

#endif // ENHANCED_LAN_DISCOVERY_H

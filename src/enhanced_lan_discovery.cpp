// Enhanced LAN Discovery für EnigmaChat
// Scannt multiple Subnets und verbindet automatisch

#include <QObject>
#include <QUdpSocket>
#include <QTcpSocket>
#include <QTimer>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <QThread>

class EnhancedLANDiscovery : public QObject
{
    Q_OBJECT
    
public:
    struct SubnetRange {
        QString baseIP;      // "192.168.0"
        int startSubnet;     // 0
        int endSubnet;       // 168
        quint16 port;        // 9090
    };
    
    explicit EnhancedLANDiscovery(QObject *parent = nullptr);
    
    // Hauptfunktionen
    void startMultiSubnetScan();
    void stopMultiSubnetScan();
    void addSubnetRange(const QString &baseIP, int startSubnet, int endSubnet, quint16 port);
    
    // Auto-Connect Features
    void enableAutoConnect(bool enabled) { m_autoConnect = enabled; }
    void setAutoConnectDelay(int seconds) { m_autoConnectDelay = seconds; }
    
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
    // Scanning state
    QList<SubnetRange> m_subnetRanges;
    int m_currentRangeIndex;
    int m_currentSubnet;
    int m_currentHost;
    QTimer *m_scanTimer;
    QUdpSocket *m_discoverySocket;
    
    // Auto-connect state
    bool m_autoConnect;
    int m_autoConnectDelay;
    QTimer *m_autoConnectTimer;
    QList<QPair<QHostAddress, quint16>> m_discoveredPeers;
    
    // Scanning parameters
    static const int HOSTS_PER_SUBNET = 254;
    static const int SCAN_TIMEOUT_MS = 50;
    
    void sendDiscoveryPacket(const QHostAddress &target, quint16 port);
    void processDiscoveryResponse(const QByteArray &data, const QHostAddress &sender, quint16 port);
    bool isValidEnigmaChatPeer(const QJsonObject &response);
};

EnhancedLANDiscovery::EnhancedLANDiscovery(QObject *parent)
    : QObject(parent)
    , m_currentRangeIndex(0)
    , m_currentSubnet(0)
    , m_currentHost(1)
    , m_autoConnect(true)
    , m_autoConnectDelay(2)
{
    // Setup discovery socket
    m_discoverySocket = new QUdpSocket(this);
    connect(m_discoverySocket, &QUdpSocket::readyRead, this, &EnhancedLANDiscovery::onDiscoveryResponse);
    
    // Setup scan timer
    m_scanTimer = new QTimer(this);
    m_scanTimer->setSingleShot(false);
    m_scanTimer->setInterval(SCAN_TIMEOUT_MS);
    connect(m_scanTimer, &QTimer::timeout, this, &EnhancedLANDiscovery::processNextSubnet);
    
    // Setup auto-connect timer
    m_autoConnectTimer = new QTimer(this);
    m_autoConnectTimer->setSingleShot(true);
    connect(m_autoConnectTimer, &QTimer::timeout, this, &EnhancedLANDiscovery::attemptAutoConnect);
    
    // Add default subnet ranges
    addSubnetRange("192.168.0", 0, 168, 9090);
    
    qDebug() << "🔍 Enhanced LAN Discovery initialized";
    qDebug() << "📊 Will scan 192.168.0.0/24 to 192.168.168.0/24 on port 9090";
}

void EnhancedLANDiscovery::addSubnetRange(const QString &baseIP, int startSubnet, int endSubnet, quint16 port)
{
    SubnetRange range;
    range.baseIP = baseIP;
    range.startSubnet = startSubnet;
    range.endSubnet = endSubnet;
    range.port = port;
    
    m_subnetRanges.append(range);
    
    int totalSubnets = endSubnet - startSubnet + 1;
    qDebug() << "📡 Added subnet range:" << baseIP << startSubnet << "to" << endSubnet << "port" << port;
    qDebug() << "📊 Total subnets to scan:" << totalSubnets << "(" << totalSubnets * HOSTS_PER_SUBNET << "hosts)";
}

void EnhancedLANDiscovery::startMultiSubnetScan()
{
    if (m_subnetRanges.isEmpty()) {
        qWarning() << "❌ No subnet ranges configured!";
        return;
    }
    
    // Reset scanning state
    m_currentRangeIndex = 0;
    m_currentSubnet = m_subnetRanges[0].startSubnet;
    m_currentHost = 1;
    m_discoveredPeers.clear();
    
    qDebug() << "🚀 Starting enhanced multi-subnet scan...";
    qDebug() << "🎯 Scanning" << m_subnetRanges.size() << "subnet ranges";
    
    // Bind discovery socket
    if (!m_discoverySocket->bind(QHostAddress::Any, 0)) {
        qWarning() << "❌ Failed to bind discovery socket:" << m_discoverySocket->errorString();
        return;
    }
    
    // Start scanning
    m_scanTimer->start();
    
    int totalSubnets = 0;
    for (const auto &range : m_subnetRanges) {
        totalSubnets += range.endSubnet - range.startSubnet + 1;
    }
    qDebug() << "📊 Total subnets to scan:" << totalSubnets;
}

void EnhancedLANDiscovery::processNextSubnet()
{
    if (m_currentRangeIndex >= m_subnetRanges.size()) {
        // Scanning completed
        m_scanTimer->stop();
        qDebug() << "✅ Multi-subnet scan completed!";
        qDebug() << "📊 Found" << m_discoveredPeers.size() << "EnigmaChat peers";
        
        emit scanCompleted(m_discoveredPeers.size());
        
        // Start auto-connect if enabled
        if (m_autoConnect && !m_discoveredPeers.isEmpty()) {
            qDebug() << "🔄 Starting auto-connect in" << m_autoConnectDelay << "seconds...";
            m_autoConnectTimer->start(m_autoConnectDelay * 1000);
        }
        return;
    }
    
    const SubnetRange &currentRange = m_subnetRanges[m_currentRangeIndex];
    
    // Build target IP
    QString targetIP = QString("%1.%2.%3")
        .arg(currentRange.baseIP)
        .arg(m_currentSubnet)
        .arg(m_currentHost);
    
    QHostAddress target(targetIP);
    
    // Send discovery packet
    sendDiscoveryPacket(target, currentRange.port);
    
    // Progress to next host
    m_currentHost++;
    
    if (m_currentHost > HOSTS_PER_SUBNET) {
        // Move to next subnet
        m_currentHost = 1;
        m_currentSubnet++;
        
        if (m_currentSubnet > currentRange.endSubnet) {
            // Move to next range
            m_currentRangeIndex++;
            if (m_currentRangeIndex < m_subnetRanges.size()) {
                m_currentSubnet = m_subnetRanges[m_currentRangeIndex].startSubnet;
            }
        }
        
        // Emit progress
        int currentSubnetNumber = 0;
        int totalSubnets = 0;
        for (int i = 0; i < m_subnetRanges.size(); i++) {
            const auto &range = m_subnetRanges[i];
            int rangeSubnets = range.endSubnet - range.startSubnet + 1;
            if (i < m_currentRangeIndex) {
                currentSubnetNumber += rangeSubnets;
            } else if (i == m_currentRangeIndex) {
                currentSubnetNumber += m_currentSubnet - range.startSubnet;
            }
            totalSubnets += rangeSubnets;
        }
        
        emit scanProgress(currentSubnetNumber, totalSubnets);
        qDebug() << "📊 Scan progress:" << currentSubnetNumber << "/" << totalSubnets << "subnets";
    }
}

void EnhancedLANDiscovery::sendDiscoveryPacket(const QHostAddress &target, quint16 port)
{
    QJsonObject discovery;
    discovery["type"] = "ENIGMA_DISCOVERY";
    discovery["client_name"] = "EnigmaChat_Scanner";
    discovery["timestamp"] = QDateTime::currentMSecsSinceEpoch();
    discovery["protocol_version"] = "1.0";
    discovery["scan_request"] = true;
    
    QJsonDocument doc(discovery);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    
    m_discoverySocket->writeDatagram(data, target, port);
}

void EnhancedLANDiscovery::onDiscoveryResponse()
{
    while (m_discoverySocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(m_discoverySocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        
        m_discoverySocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        processDiscoveryResponse(datagram, sender, senderPort);
    }
}

void EnhancedLANDiscovery::processDiscoveryResponse(const QByteArray &data, const QHostAddress &sender, quint16 port)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError) {
        return; // Invalid JSON
    }
    
    QJsonObject response = doc.object();
    
    if (isValidEnigmaChatPeer(response)) {
        QString peerName = response["client_name"].toString();
        
        qDebug() << "🎯 EnigmaChat peer discovered:" << peerName << "at" << sender.toString() << ":" << port;
        
        // Add to discovered peers
        m_discoveredPeers.append(qMakePair(sender, port));
        
        emit peerDiscovered(peerName, sender, port);
    }
}

bool EnhancedLANDiscovery::isValidEnigmaChatPeer(const QJsonObject &response)
{
    return response["type"].toString() == "ENIGMA_RESPONSE" &&
           response.contains("client_name") &&
           response.contains("protocol_version") &&
           response["protocol_version"].toString() == "1.0";
}

void EnhancedLANDiscovery::attemptAutoConnect()
{
    if (m_discoveredPeers.isEmpty()) {
        qDebug() << "ℹ️ No peers available for auto-connect";
        return;
    }
    
    qDebug() << "🔄 Attempting auto-connect to" << m_discoveredPeers.size() << "peers...";
    
    for (const auto &peerInfo : m_discoveredPeers) {
        QHostAddress ip = peerInfo.first;
        quint16 port = peerInfo.second;
        
        // Attempt TCP connection
        QTcpSocket *socket = new QTcpSocket(this);
        
        connect(socket, &QTcpSocket::connected, [this, ip, port, socket]() {
            qDebug() << "✅ Auto-connected to peer at" << ip.toString() << ":" << port;
            emit peerConnected("AutoConnected_" + ip.toString(), ip, port);
            
            // TODO: Perform handshake and integrate with main client
        });
        
        connect(socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                [ip, port](QAbstractSocket::SocketError error) {
            qDebug() << "❌ Auto-connect failed to" << ip.toString() << ":" << port << "Error:" << error;
        });
        
        qDebug() << "🔗 Attempting auto-connect to" << ip.toString() << ":" << port;
        socket->connectToHost(ip, port);
    }
}

void EnhancedLANDiscovery::stopMultiSubnetScan()
{
    m_scanTimer->stop();
    m_autoConnectTimer->stop();
    m_discoverySocket->close();
    
    qDebug() << "⏹️ Multi-subnet scan stopped";
}

#include "enhanced_lan_discovery.moc"

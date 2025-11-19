#ifndef NETWORKSERVICE_H
#define NETWORKSERVICE_H

#include <QString>
#include <QMetaType>

struct NetworkService {
    QString ip;
    int port = 0;
    QString service;     // e.g., "FTP", "NFS"
    QString type;        // alias for service
    QString serviceName; // display-friendly name
    QString status;
    int responseTime = 0; // ms
};

Q_DECLARE_METATYPE(NetworkService)

#endif // NETWORKSERVICE_H

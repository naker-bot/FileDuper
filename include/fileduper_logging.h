// fileduper_logging.h
// Zentrale Logging-Steuerung über Environment-Variable FILEDUPER_LOG_LEVEL
// 0 = Errors/Warnungen, 1 = Info + Errors, 2 = Voller Debug (Default falls unset)

#ifndef FILEDUPER_LOGGING_H
#define FILEDUPER_LOGGING_H

#include <QDebug>
#include <QtGlobal>

inline int fileduperLogLevel()
{
    static int s_level = [](){
        QByteArray v = qgetenv("FILEDUPER_LOG_LEVEL");
        if (v.isEmpty()) return 2; // Default: volle Diagnose bis reduziert wird
        bool ok = false; int val = v.toInt(&ok); if(!ok) return 2; if (val < 0) val = 0; if (val > 2) val = 2; return val; }();
    return s_level;
}

// Makros für selektives Logging
#define FD_LOG_DEBUG() if (fileduperLogLevel() >= 2) qDebug()
#define FD_LOG_INFO()  if (fileduperLogLevel() >= 1) qInfo()
#define FD_LOG_WARN()  qWarning()
#define FD_LOG_ERROR() qWarning()

// Hilfs-Makro für extrem häufige Schleifen-Logs (kann komplett deaktiviert werden)
#define FD_LOG_SPAM()  if (fileduperLogLevel() >= 2) qDebug()

#endif // FILEDUPER_LOGGING_H

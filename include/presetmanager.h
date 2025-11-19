#ifndef PRESETMANAGER_H
#define PRESETMANAGER_H

#include <QString>
#include <QMap>
#include <QDateTime>

struct LoginData {
    QString username;
    QString password;
    QString service;
    bool saveCredentials = false;
    bool autoLogin = false;
    QDateTime lastUsed;
};

class PresetManager {
public:
    PresetManager() {
        qDebug() << "[PresetManager] Initialized";
    }
    ~PresetManager() {}
    
    // Login Credential Management
    void saveLogin(const QString &host, int port, const LoginData &login) {
        QString key = QString("%1:%2").arg(host).arg(port);
        m_loginCache[key] = login;
        qDebug() << "[PresetManager] 💾 Saved login for" << key 
                 << "username:" << login.username 
                 << "save:" << login.saveCredentials
                 << "autoLogin:" << login.autoLogin;
    }
    
    LoginData getLogin(const QString &host, int port) const {
        QString key = QString("%1:%2").arg(host).arg(port);
        
        if (m_loginCache.contains(key)) {
            const LoginData &login = m_loginCache[key];
            qDebug() << "[PresetManager] 📂 Retrieved login for" << key 
                     << "username:" << login.username;
            return login;
        }
        
        qDebug() << "[PresetManager] ⚠️ No login found for" << key;
        LoginData empty;
        empty.username = "";
        empty.password = "";
        return empty;
    }
    
    void deleteLogin(const QString &host, int port) {
        QString key = QString("%1:%2").arg(host).arg(port);
        if (m_loginCache.remove(key) > 0) {
            qDebug() << "[PresetManager] 🗑️ Deleted login for" << key;
        }
    }
    
    bool hasLogin(const QString &host, int port) const {
        QString key = QString("%1:%2").arg(host).arg(port);
        return m_loginCache.contains(key);
    }
    
private:
    mutable QMap<QString, LoginData> m_loginCache;  // host:port -> LoginData
};

#endif // PRESETMANAGER_H

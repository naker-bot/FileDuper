#include "preferences.h"
#include <QStandardPaths>
#include <QDebug>

Preferences& Preferences::instance()
{
    static Preferences instance;
    return instance;
}

Preferences::Preferences()
    : m_settings("FileDuper", "FileDuper")
{
    qDebug() << "[Preferences] 📋 Preferences-System initialisiert";
    qDebug() << "[Preferences] 📁 Settings-Pfad:" << m_settings.fileName();
}

QString Preferences::getTheme() const
{
    return m_settings.value("theme", "Dark Modern").toString();
}

void Preferences::setTheme(const QString &theme)
{
    m_settings.setValue("theme", theme);
    qDebug() << "[Preferences] 🎨 Theme gespeichert:" << theme;
}

int Preferences::getWindowWidth() const
{
    return m_settings.value("window/width", 1200).toInt();
}

int Preferences::getWindowHeight() const
{
    return m_settings.value("window/height", 800).toInt();
}

int Preferences::getWindowX() const
{
    return m_settings.value("window/x", 100).toInt();
}

int Preferences::getWindowY() const
{
    return m_settings.value("window/y", 100).toInt();
}

bool Preferences::getWindowMaximized() const
{
    return m_settings.value("window/maximized", false).toBool();
}

void Preferences::setWindowGeometry(int w, int h, int x, int y, bool maximized)
{
    m_settings.setValue("window/width", w);
    m_settings.setValue("window/height", h);
    m_settings.setValue("window/x", x);
    m_settings.setValue("window/y", y);
    m_settings.setValue("window/maximized", maximized);
    qDebug() << "[Preferences] 🪟 Fenster-Geometrie gespeichert";
}

bool Preferences::getAutoScan() const
{
    return m_settings.value("scanner/autoscan", false).toBool();
}

void Preferences::setAutoScan(bool enabled)
{
    m_settings.setValue("scanner/autoscan", enabled);
}

int Preferences::getThreadCount() const
{
    return m_settings.value("scanner/threads", 4).toInt();
}

void Preferences::setThreadCount(int count)
{
    m_settings.setValue("scanner/threads", count);
}

QString Preferences::getLastScanPath() const
{
    return m_settings.value("scanner/lastpath", QStandardPaths::writableLocation(QStandardPaths::HomeLocation)).toString();
}

void Preferences::setLastScanPath(const QString &path)
{
    m_settings.setValue("scanner/lastpath", path);
}

QString Preferences::getDefaultHashAlgorithm() const
{
    return m_settings.value("hash/algorithm", "MD5").toString();
}

void Preferences::setDefaultHashAlgorithm(const QString &algo)
{
    m_settings.setValue("hash/algorithm", algo);
}

bool Preferences::getShowTips() const
{
    return m_settings.value("ui/showtips", true).toBool();
}

void Preferences::setShowTips(bool show)
{
    m_settings.setValue("ui/showtips", show);
}

bool Preferences::getDarkMode() const
{
    return m_settings.value("ui/darkmode", true).toBool();
}

void Preferences::setDarkMode(bool dark)
{
    m_settings.setValue("ui/darkmode", dark);
}

void Preferences::sync()
{
    m_settings.sync();
    qDebug() << "[Preferences] 💾 Einstellungen gespeichert";
}

void Preferences::reset()
{
    m_settings.clear();
    m_settings.sync();
    qDebug() << "[Preferences] ↻ Einstellungen zurückgesetzt";
}

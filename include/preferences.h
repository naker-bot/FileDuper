#ifndef PREFERENCES_H
#define PREFERENCES_H

#include <QString>
#include <QSettings>
#include <QColor>

class Preferences {
public:
    static Preferences& instance();
    
    // Theme
    QString getTheme() const;
    void setTheme(const QString &theme);
    
    // Fenster
    int getWindowWidth() const;
    int getWindowHeight() const;
    int getWindowX() const;
    int getWindowY() const;
    bool getWindowMaximized() const;
    
    void setWindowGeometry(int w, int h, int x, int y, bool maximized);
    
    // Scanner
    bool getAutoScan() const;
    void setAutoScan(bool enabled);
    
    int getThreadCount() const;
    void setThreadCount(int count);
    
    QString getLastScanPath() const;
    void setLastScanPath(const QString &path);
    
    // Hash
    QString getDefaultHashAlgorithm() const;
    void setDefaultHashAlgorithm(const QString &algo);
    
    // GUI
    bool getShowTips() const;
    void setShowTips(bool show);
    
    bool getDarkMode() const;
    void setDarkMode(bool dark);
    
    // Speichern/Laden
    void sync();
    void reset();

private:
    Preferences();
    ~Preferences() = default;
    
    Preferences(const Preferences&) = delete;
    Preferences& operator=(const Preferences&) = delete;
    
    QSettings m_settings;
};

#endif // PREFERENCES_H

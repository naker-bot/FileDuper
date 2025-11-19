#ifndef NETWORKPRESETMANAGER_H
#define NETWORKPRESETMANAGER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QSettings>
#include <QMap>
#include <QDateTime>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>

struct NetworkPreset {
    QString name;
    QString cidr;
    QString description;
    QString interfaceName;
    QString category;          // "Home", "Office", "Lab", "Production"
    bool isActive;
    QDateTime createdDate;
    QDateTime lastUsed;
    int usageCount;
    QStringList tags;          // Custom tags for organization
    QString color;             // Color coding for UI
    bool isReadOnly;           // System presets that can't be deleted
    
    NetworkPreset() : isActive(false), usageCount(0), isReadOnly(false) {
        createdDate = QDateTime::currentDateTime();
    }
    
    bool isValid() const {
        return !name.isEmpty() && !cidr.isEmpty();
    }
    
    QJsonObject toJson() const;
    static NetworkPreset fromJson(const QJsonObject &json);
};

struct NetworkGroup
{
    QString name;
    QString description;
    QStringList presetNames;
    QString color;
    bool isExpanded;  // UI state
    
    NetworkGroup() : isExpanded(true) {}
};

class NetworkPresetManager : public QObject
{
    Q_OBJECT

public:
    explicit NetworkPresetManager(QObject *parent = nullptr);
    ~NetworkPresetManager();

    // Preset Management
    bool addPreset(const NetworkPreset &preset);
    bool removePreset(const QString &name);
    bool updatePreset(const QString &name, const NetworkPreset &preset);
    NetworkPreset getPreset(const QString &name) const;
    QList<NetworkPreset> getAllPresets() const;
    QStringList getPresetNames() const;
    QStringList getPresetsByCategory(const QString &category) const;
    QStringList getPresetsByTag(const QString &tag) const;
    
    // Active Preset Management
    bool setActivePreset(const QString &name);
    QString getActivePreset() const;
    NetworkPreset getActivePresetData() const;
    
    // Group Management
    bool addGroup(const NetworkGroup &group);
    bool removeGroup(const QString &name);
    bool updateGroup(const QString &name, const NetworkGroup &group);
    NetworkGroup getGroup(const QString &name) const;
    QList<NetworkGroup> getAllGroups() const;
    QStringList getGroupNames() const;
    bool addPresetToGroup(const QString &presetName, const QString &groupName);
    bool removePresetFromGroup(const QString &presetName, const QString &groupName);
    
    // Quick Access Presets
    QStringList getFavoritePresets() const;
    bool addToFavorites(const QString &presetName);
    bool removeFromFavorites(const QString &presetName);
    QStringList getRecentlyUsed(int count = 5) const;
    
    // Preset Categories
    QStringList getAvailableCategories() const;
    bool addCategory(const QString &category, const QString &description = "");
    bool removeCategory(const QString &category);
    
    // Import/Export
    bool exportPresets(const QString &filePath) const;
    bool importPresets(const QString &filePath, bool overwrite = false);
    bool exportGroup(const QString &groupName, const QString &filePath) const;
    bool importGroup(const QString &filePath);
    
    // Backup/Restore
    bool createBackup(const QString &filePath = "") const;
    bool restoreFromBackup(const QString &filePath);
    QStringList getAvailableBackups() const;
    
    // Search and Filter
    QStringList searchPresets(const QString &query) const;
    QStringList filterByUsage(int minUsage) const;
    QStringList filterByDate(const QDateTime &since) const;
    
    // Statistics
    NetworkPreset getMostUsedPreset() const;
    QMap<QString, int> getCategoryUsageStats() const;
    int getTotalUsageCount() const;
    
    // Validation
    bool validateCidr(const QString &cidr) const;
    bool isPresetNameAvailable(const QString &name) const;
    QStringList validatePreset(const NetworkPreset &preset) const;
    
    // Settings Management
    void loadSettings();
    void saveSettings();
    void resetToDefaults();
    
    // Configuration
    QString getConfigFilePath() const;
    void setConfigFilePath(const QString &path);
    bool isAutoSaveEnabled() const;
    void setAutoSaveEnabled(bool enabled);
    int getAutoSaveInterval() const;
    void setAutoSaveInterval(int seconds);

signals:
    void presetAdded(const QString &name);
    void presetRemoved(const QString &name);
    void presetUpdated(const QString &name);
    void activePresetChanged(const QString &name);
    void groupAdded(const QString &name);
    void groupRemoved(const QString &name);
    void groupUpdated(const QString &name);
    void settingsLoaded();
    void settingsSaved();
    void backupCreated(const QString &filePath);
    void importCompleted(int count);

private slots:
    void onPresetUsed(const QString &name);
    void autoSave();

private:
    void initializeDefaults();
    void createDefaultPresets();
    void createDefaultGroups();
    void migrateOldSettings();
    void startAutoSaveTimer();
    void stopAutoSaveTimer();
    
    QString generateUniquePresetName(const QString &baseName) const;
    void updatePresetUsage(const QString &name);
    bool isValidPresetName(const QString &name) const;
    
    QSettings *m_settings;
    QString m_configFilePath;
    QMap<QString, NetworkPreset> m_presets;
    QMap<QString, NetworkGroup> m_groups;
    QStringList m_favorites;
    QStringList m_recentlyUsed;
    QMap<QString, QString> m_categories;
    QString m_activePreset;
    
    QTimer *m_autoSaveTimer;
    bool m_autoSaveEnabled;
    int m_autoSaveInterval;
    bool m_isDirty;
    
    static const QString PRESETS_GROUP;
    static const QString GROUPS_GROUP;
    static const QString FAVORITES_GROUP;
    static const QString RECENT_GROUP;
    static const QString CATEGORIES_GROUP;
    static const QString GENERAL_GROUP;
    static const int MAX_RECENT_COUNT;
};

#endif // NETWORKPRESETMANAGER_H
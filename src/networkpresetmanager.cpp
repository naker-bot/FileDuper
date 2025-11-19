#include "networkpresetmanager.h"
#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include <QHostAddress>
#include <QRegularExpression>
#include <iostream>

const QString NetworkPresetManager::PRESETS_GROUP = "NetworkPresets";
const QString NetworkPresetManager::GROUPS_GROUP = "PresetGroups";
const QString NetworkPresetManager::FAVORITES_GROUP = "Favorites";
const QString NetworkPresetManager::RECENT_GROUP = "RecentlyUsed";
const QString NetworkPresetManager::CATEGORIES_GROUP = "Categories";
const QString NetworkPresetManager::GENERAL_GROUP = "General";
const int NetworkPresetManager::MAX_RECENT_COUNT = 10;

QJsonObject NetworkPreset::toJson() const
{
    QJsonObject obj;
    obj["name"] = name;
    obj["cidr"] = cidr;
    obj["description"] = description;
    obj["interfaceName"] = interfaceName;
    obj["category"] = category;
    obj["isActive"] = isActive;
    obj["createdDate"] = createdDate.toString(Qt::ISODate);
    obj["lastUsed"] = lastUsed.toString(Qt::ISODate);
    obj["usageCount"] = usageCount;
    obj["color"] = color;
    obj["isReadOnly"] = isReadOnly;
    
    QJsonArray tagsArray;
    for (const QString &tag : tags) {
        tagsArray.append(tag);
    }
    obj["tags"] = tagsArray;
    
    return obj;
}

NetworkPreset NetworkPreset::fromJson(const QJsonObject &json)
{
    NetworkPreset preset;
    preset.name = json["name"].toString();
    preset.cidr = json["cidr"].toString();
    preset.description = json["description"].toString();
    preset.interfaceName = json["interfaceName"].toString();
    preset.category = json["category"].toString();
    preset.isActive = json["isActive"].toBool();
    preset.createdDate = QDateTime::fromString(json["createdDate"].toString(), Qt::ISODate);
    preset.lastUsed = QDateTime::fromString(json["lastUsed"].toString(), Qt::ISODate);
    preset.usageCount = json["usageCount"].toInt();
    preset.color = json["color"].toString();
    preset.isReadOnly = json["isReadOnly"].toBool();
    
    QJsonArray tagsArray = json["tags"].toArray();
    for (const QJsonValue &value : tagsArray) {
        preset.tags.append(value.toString());
    }
    
    return preset;
}

NetworkPresetManager::NetworkPresetManager(QObject *parent)
    : QObject(parent)
    , m_settings(nullptr)
    , m_autoSaveTimer(nullptr)
    , m_autoSaveEnabled(true)
    , m_autoSaveInterval(30)
    , m_isDirty(false)
{
    // Setup config file path
    QString configDir = QStandardPaths::writableLocation(QStandardPaths::ConfigLocation);
    QDir().mkpath(configDir);
    m_configFilePath = configDir + "/FileDuper/network_presets.ini";
    
    // Initialize settings
    m_settings = new QSettings(m_configFilePath, QSettings::IniFormat, this);
    
    // Initialize defaults and load settings
    initializeDefaults();
    loadSettings();
    
    // Start auto-save timer
    if (m_autoSaveEnabled) {
        startAutoSaveTimer();
    }
    
    qDebug() << "[NetworkPresetManager] 🚀 Initialized with config:" << m_configFilePath;
}

NetworkPresetManager::~NetworkPresetManager()
{
    if (m_isDirty) {
        saveSettings();
    }
    stopAutoSaveTimer();
}

bool NetworkPresetManager::addPreset(const NetworkPreset &preset)
{
    if (!preset.isValid()) {
        qWarning() << "[NetworkPresetManager] ❌ Invalid preset:" << preset.name;
        return false;
    }
    
    if (m_presets.contains(preset.name)) {
        qWarning() << "[NetworkPresetManager] ❌ Preset already exists:" << preset.name;
        return false;
    }
    
    NetworkPreset newPreset = preset;
    newPreset.createdDate = QDateTime::currentDateTime();
    
    m_presets[preset.name] = newPreset;
    m_isDirty = true;
    
    qDebug() << "[NetworkPresetManager] ✅ Added preset:" << preset.name << "CIDR:" << preset.cidr;
    emit presetAdded(preset.name);
    
    return true;
}

bool NetworkPresetManager::removePreset(const QString &name)
{
    if (!m_presets.contains(name)) {
        qWarning() << "[NetworkPresetManager] ❌ Preset not found:" << name;
        return false;
    }
    
    NetworkPreset preset = m_presets[name];
    if (preset.isReadOnly) {
        qWarning() << "[NetworkPresetManager] ❌ Cannot remove read-only preset:" << name;
        return false;
    }
    
    m_presets.remove(name);
    m_favorites.removeAll(name);
    m_recentlyUsed.removeAll(name);
    
    // Remove from groups
    for (auto &group : m_groups) {
        group.presetNames.removeAll(name);
    }
    
    // Clear active preset if it was the removed one
    if (m_activePreset == name) {
        m_activePreset.clear();
    }
    
    m_isDirty = true;
    
    qDebug() << "[NetworkPresetManager] ✅ Removed preset:" << name;
    emit presetRemoved(name);
    
    return true;
}

bool NetworkPresetManager::updatePreset(const QString &name, const NetworkPreset &preset)
{
    if (!m_presets.contains(name)) {
        qWarning() << "[NetworkPresetManager] ❌ Preset not found for update:" << name;
        return false;
    }
    
    if (!preset.isValid()) {
        qWarning() << "[NetworkPresetManager] ❌ Invalid preset data for update:" << name;
        return false;
    }
    
    NetworkPreset existingPreset = m_presets[name];
    if (existingPreset.isReadOnly) {
        qWarning() << "[NetworkPresetManager] ❌ Cannot update read-only preset:" << name;
        return false;
    }
    
    // Preserve creation date and usage stats
    NetworkPreset updatedPreset = preset;
    updatedPreset.createdDate = existingPreset.createdDate;
    updatedPreset.usageCount = existingPreset.usageCount;
    updatedPreset.lastUsed = existingPreset.lastUsed;
    
    m_presets[name] = updatedPreset;
    m_isDirty = true;
    
    qDebug() << "[NetworkPresetManager] ✅ Updated preset:" << name;
    emit presetUpdated(name);
    
    return true;
}

NetworkPreset NetworkPresetManager::getPreset(const QString &name) const
{
    return m_presets.value(name);
}

QList<NetworkPreset> NetworkPresetManager::getAllPresets() const
{
    return m_presets.values();
}

QStringList NetworkPresetManager::getPresetNames() const
{
    return m_presets.keys();
}

QStringList NetworkPresetManager::getPresetsByCategory(const QString &category) const
{
    QStringList presets;
    for (auto it = m_presets.constBegin(); it != m_presets.constEnd(); ++it) {
        if (it.value().category == category) {
            presets.append(it.key());
        }
    }
    return presets;
}

QStringList NetworkPresetManager::getPresetsByTag(const QString &tag) const
{
    QStringList presets;
    for (auto it = m_presets.constBegin(); it != m_presets.constEnd(); ++it) {
        if (it.value().tags.contains(tag)) {
            presets.append(it.key());
        }
    }
    return presets;
}

bool NetworkPresetManager::setActivePreset(const QString &name)
{
    if (!name.isEmpty() && !m_presets.contains(name)) {
        qWarning() << "[NetworkPresetManager] ❌ Cannot set active preset - not found:" << name;
        return false;
    }
    
    QString oldActive = m_activePreset;
    m_activePreset = name;
    
    if (!name.isEmpty()) {
        updatePresetUsage(name);
    }
    
    m_isDirty = true;
    
    qDebug() << "[NetworkPresetManager] 🎯 Active preset changed:" << oldActive << "→" << name;
    emit activePresetChanged(name);
    
    return true;
}

QString NetworkPresetManager::getActivePreset() const
{
    return m_activePreset;
}

NetworkPreset NetworkPresetManager::getActivePresetData() const
{
    return m_presets.value(m_activePreset);
}

bool NetworkPresetManager::addGroup(const NetworkGroup &group)
{
    if (group.name.isEmpty()) {
        qWarning() << "[NetworkPresetManager] ❌ Invalid group name";
        return false;
    }
    
    if (m_groups.contains(group.name)) {
        qWarning() << "[NetworkPresetManager] ❌ Group already exists:" << group.name;
        return false;
    }
    
    m_groups[group.name] = group;
    m_isDirty = true;
    
    qDebug() << "[NetworkPresetManager] ✅ Added group:" << group.name;
    emit groupAdded(group.name);
    
    return true;
}

bool NetworkPresetManager::removeGroup(const QString &name)
{
    if (!m_groups.contains(name)) {
        qWarning() << "[NetworkPresetManager] ❌ Group not found:" << name;
        return false;
    }
    
    m_groups.remove(name);
    m_isDirty = true;
    
    qDebug() << "[NetworkPresetManager] ✅ Removed group:" << name;
    emit groupRemoved(name);
    
    return true;
}

bool NetworkPresetManager::updateGroup(const QString &name, const NetworkGroup &group)
{
    if (!m_groups.contains(name)) {
        qWarning() << "[NetworkPresetManager] ❌ Group not found for update:" << name;
        return false;
    }
    
    m_groups[name] = group;
    m_isDirty = true;
    
    qDebug() << "[NetworkPresetManager] ✅ Updated group:" << name;
    emit groupUpdated(name);
    
    return true;
}

NetworkGroup NetworkPresetManager::getGroup(const QString &name) const
{
    return m_groups.value(name);
}

QList<NetworkGroup> NetworkPresetManager::getAllGroups() const
{
    return m_groups.values();
}

QStringList NetworkPresetManager::getGroupNames() const
{
    return m_groups.keys();
}

bool NetworkPresetManager::addPresetToGroup(const QString &presetName, const QString &groupName)
{
    if (!m_presets.contains(presetName)) {
        qWarning() << "[NetworkPresetManager] ❌ Preset not found:" << presetName;
        return false;
    }
    
    if (!m_groups.contains(groupName)) {
        qWarning() << "[NetworkPresetManager] ❌ Group not found:" << groupName;
        return false;
    }
    
    if (!m_groups[groupName].presetNames.contains(presetName)) {
        m_groups[groupName].presetNames.append(presetName);
        m_isDirty = true;
        
        qDebug() << "[NetworkPresetManager] ✅ Added preset to group:" << presetName << "→" << groupName;
        emit groupUpdated(groupName);
    }
    
    return true;
}

bool NetworkPresetManager::removePresetFromGroup(const QString &presetName, const QString &groupName)
{
    if (!m_groups.contains(groupName)) {
        qWarning() << "[NetworkPresetManager] ❌ Group not found:" << groupName;
        return false;
    }
    
    if (m_groups[groupName].presetNames.removeAll(presetName) > 0) {
        m_isDirty = true;
        
        qDebug() << "[NetworkPresetManager] ✅ Removed preset from group:" << presetName << "←" << groupName;
        emit groupUpdated(groupName);
        return true;
    }
    
    return false;
}

QStringList NetworkPresetManager::getFavoritePresets() const
{
    return m_favorites;
}

bool NetworkPresetManager::addToFavorites(const QString &presetName)
{
    if (!m_presets.contains(presetName)) {
        qWarning() << "[NetworkPresetManager] ❌ Cannot add to favorites - preset not found:" << presetName;
        return false;
    }
    
    if (!m_favorites.contains(presetName)) {
        m_favorites.append(presetName);
        m_isDirty = true;
        
        qDebug() << "[NetworkPresetManager] ⭐ Added to favorites:" << presetName;
    }
    
    return true;
}

bool NetworkPresetManager::removeFromFavorites(const QString &presetName)
{
    if (m_favorites.removeAll(presetName) > 0) {
        m_isDirty = true;
        
        qDebug() << "[NetworkPresetManager] ✅ Removed from favorites:" << presetName;
        return true;
    }
    
    return false;
}

QStringList NetworkPresetManager::getRecentlyUsed(int count) const
{
    return m_recentlyUsed.mid(0, qMin(count, m_recentlyUsed.size()));
}

QStringList NetworkPresetManager::getAvailableCategories() const
{
    return m_categories.keys();
}

bool NetworkPresetManager::addCategory(const QString &category, const QString &description)
{
    if (category.isEmpty()) {
        return false;
    }
    
    m_categories[category] = description;
    m_isDirty = true;
    
    qDebug() << "[NetworkPresetManager] ✅ Added category:" << category;
    return true;
}

bool NetworkPresetManager::removeCategory(const QString &category)
{
    if (m_categories.remove(category) > 0) {
        m_isDirty = true;
        
        qDebug() << "[NetworkPresetManager] ✅ Removed category:" << category;
        return true;
    }
    
    return false;
}

bool NetworkPresetManager::validateCidr(const QString &cidr) const
{
    QRegularExpression cidrRegex(R"(^(\d{1,3})\.(\d{1,3})\.(\d{1,3})\.(\d{1,3})/(\d{1,2})$)");
    QRegularExpressionMatch match = cidrRegex.match(cidr);
    
    if (!match.hasMatch()) {
        return false;
    }
    
    // Validate IP octets
    for (int i = 1; i <= 4; ++i) {
        bool ok;
        int octet = match.captured(i).toInt(&ok);
        if (!ok || octet < 0 || octet > 255) {
            return false;
        }
    }
    
    // Validate subnet mask
    bool ok;
    int mask = match.captured(5).toInt(&ok);
    if (!ok || mask < 0 || mask > 32) {
        return false;
    }
    
    return true;
}

bool NetworkPresetManager::isPresetNameAvailable(const QString &name) const
{
    return !m_presets.contains(name);
}

QStringList NetworkPresetManager::validatePreset(const NetworkPreset &preset) const
{
    QStringList errors;
    
    if (preset.name.isEmpty()) {
        errors.append("Preset name cannot be empty");
    }
    
    if (preset.cidr.isEmpty()) {
        errors.append("CIDR cannot be empty");
    } else if (!validateCidr(preset.cidr)) {
        errors.append("Invalid CIDR format");
    }
    
    if (!preset.name.isEmpty() && m_presets.contains(preset.name)) {
        errors.append("Preset name already exists");
    }
    
    return errors;
}

void NetworkPresetManager::loadSettings()
{
    qDebug() << "[NetworkPresetManager] 📁 Loading settings from:" << m_configFilePath;
    
    // Load presets
    m_settings->beginGroup(PRESETS_GROUP);
    QStringList presetKeys = m_settings->childGroups();
    for (const QString &key : presetKeys) {
        m_settings->beginGroup(key);
        
        NetworkPreset preset;
        preset.name = m_settings->value("name").toString();
        preset.cidr = m_settings->value("cidr").toString();
        preset.description = m_settings->value("description").toString();
        preset.interfaceName = m_settings->value("interfaceName").toString();
        preset.category = m_settings->value("category", "General").toString();
        preset.isActive = m_settings->value("isActive", false).toBool();
        preset.createdDate = m_settings->value("createdDate", QDateTime::currentDateTime()).toDateTime();
        preset.lastUsed = m_settings->value("lastUsed").toDateTime();
        preset.usageCount = m_settings->value("usageCount", 0).toInt();
        preset.tags = m_settings->value("tags").toStringList();
        preset.color = m_settings->value("color").toString();
        preset.isReadOnly = m_settings->value("isReadOnly", false).toBool();
        
        if (preset.isValid()) {
            m_presets[preset.name] = preset;
        }
        
        m_settings->endGroup();
    }
    m_settings->endGroup();
    
    // Load groups
    m_settings->beginGroup(GROUPS_GROUP);
    QStringList groupKeys = m_settings->childGroups();
    for (const QString &key : groupKeys) {
        m_settings->beginGroup(key);
        
        NetworkGroup group;
        group.name = m_settings->value("name").toString();
        group.description = m_settings->value("description").toString();
        group.presetNames = m_settings->value("presetNames").toStringList();
        group.color = m_settings->value("color").toString();
        group.isExpanded = m_settings->value("isExpanded", true).toBool();
        
        if (!group.name.isEmpty()) {
            m_groups[group.name] = group;
        }
        
        m_settings->endGroup();
    }
    m_settings->endGroup();
    
    // Load other data
    m_settings->beginGroup(GENERAL_GROUP);
    m_activePreset = m_settings->value("activePreset").toString();
    m_autoSaveEnabled = m_settings->value("autoSaveEnabled", true).toBool();
    m_autoSaveInterval = m_settings->value("autoSaveInterval", 30).toInt();
    m_settings->endGroup();
    
    m_settings->beginGroup(FAVORITES_GROUP);
    m_favorites = m_settings->value("presets").toStringList();
    m_settings->endGroup();
    
    m_settings->beginGroup(RECENT_GROUP);
    m_recentlyUsed = m_settings->value("presets").toStringList();
    m_settings->endGroup();
    
    m_settings->beginGroup(CATEGORIES_GROUP);
    QStringList categoryKeys = m_settings->childKeys();
    for (const QString &key : categoryKeys) {
        m_categories[key] = m_settings->value(key).toString();
    }
    m_settings->endGroup();
    
    m_isDirty = false;
    
    qDebug() << "[NetworkPresetManager] ✅ Loaded" << m_presets.size() << "presets," 
             << m_groups.size() << "groups," << m_categories.size() << "categories";
    
    emit settingsLoaded();
}

void NetworkPresetManager::saveSettings()
{
    if (!m_isDirty) {
        return;
    }
    
    qDebug() << "[NetworkPresetManager] 💾 Saving settings to:" << m_configFilePath;
    
    // Clear existing data
    m_settings->clear();
    
    // Save presets
    m_settings->beginGroup(PRESETS_GROUP);
    int presetIndex = 0;
    for (auto it = m_presets.constBegin(); it != m_presets.constEnd(); ++it) {
        const NetworkPreset &preset = it.value();
        QString groupKey = QString("preset_%1").arg(presetIndex++);
        
        m_settings->beginGroup(groupKey);
        m_settings->setValue("name", preset.name);
        m_settings->setValue("cidr", preset.cidr);
        m_settings->setValue("description", preset.description);
        m_settings->setValue("interfaceName", preset.interfaceName);
        m_settings->setValue("category", preset.category);
        m_settings->setValue("isActive", preset.isActive);
        m_settings->setValue("createdDate", preset.createdDate);
        m_settings->setValue("lastUsed", preset.lastUsed);
        m_settings->setValue("usageCount", preset.usageCount);
        m_settings->setValue("tags", preset.tags);
        m_settings->setValue("color", preset.color);
        m_settings->setValue("isReadOnly", preset.isReadOnly);
        m_settings->endGroup();
    }
    m_settings->endGroup();
    
    // Save groups
    m_settings->beginGroup(GROUPS_GROUP);
    int groupIndex = 0;
    for (auto it = m_groups.constBegin(); it != m_groups.constEnd(); ++it) {
        const NetworkGroup &group = it.value();
        QString groupKey = QString("group_%1").arg(groupIndex++);
        
        m_settings->beginGroup(groupKey);
        m_settings->setValue("name", group.name);
        m_settings->setValue("description", group.description);
        m_settings->setValue("presetNames", group.presetNames);
        m_settings->setValue("color", group.color);
        m_settings->setValue("isExpanded", group.isExpanded);
        m_settings->endGroup();
    }
    m_settings->endGroup();
    
    // Save other data
    m_settings->beginGroup(GENERAL_GROUP);
    m_settings->setValue("activePreset", m_activePreset);
    m_settings->setValue("autoSaveEnabled", m_autoSaveEnabled);
    m_settings->setValue("autoSaveInterval", m_autoSaveInterval);
    m_settings->endGroup();
    
    m_settings->beginGroup(FAVORITES_GROUP);
    m_settings->setValue("presets", m_favorites);
    m_settings->endGroup();
    
    m_settings->beginGroup(RECENT_GROUP);
    m_settings->setValue("presets", m_recentlyUsed);
    m_settings->endGroup();
    
    m_settings->beginGroup(CATEGORIES_GROUP);
    for (auto it = m_categories.constBegin(); it != m_categories.constEnd(); ++it) {
        m_settings->setValue(it.key(), it.value());
    }
    m_settings->endGroup();
    
    m_settings->sync();
    m_isDirty = false;
    
    qDebug() << "[NetworkPresetManager] ✅ Settings saved successfully";
    emit settingsSaved();
}

void NetworkPresetManager::initializeDefaults()
{
    // Create default categories
    m_categories["Home"] = "Home network configurations";
    m_categories["Office"] = "Office and workplace networks";
    m_categories["Lab"] = "Laboratory and testing environments";
    m_categories["Production"] = "Production and live environments";
    m_categories["Guest"] = "Guest and temporary networks";
    m_categories["VPN"] = "VPN and remote access networks";
    m_categories["Cloud"] = "Cloud provider networks";
    m_categories["General"] = "General purpose configurations";
    
    createDefaultPresets();
    createDefaultGroups();
}

void NetworkPresetManager::createDefaultPresets()
{
    // Only create defaults if no presets exist
    if (!m_presets.isEmpty()) {
        return;
    }
    
    // Common home networks
    NetworkPreset homePreset;
    homePreset.name = "Home Network (192.168.1.x)";
    homePreset.cidr = "192.168.1.0/24";
    homePreset.description = "Standard home router network";
    homePreset.category = "Home";
    homePreset.color = "#4CAF50";
    homePreset.isReadOnly = true;
    m_presets[homePreset.name] = homePreset;
    
    NetworkPreset homePreset2;
    homePreset2.name = "Home Network (192.168.0.x)";
    homePreset2.cidr = "192.168.0.0/24";
    homePreset2.description = "Alternative home router network";
    homePreset2.category = "Home";
    homePreset2.color = "#4CAF50";
    homePreset2.isReadOnly = true;
    m_presets[homePreset2.name] = homePreset2;
    
    // Office networks
    NetworkPreset officePreset;
    officePreset.name = "Office Network (10.0.0.x)";
    officePreset.cidr = "10.0.0.0/16";
    officePreset.description = "Large office network";
    officePreset.category = "Office";
    officePreset.color = "#2196F3";
    officePreset.isReadOnly = true;
    m_presets[officePreset.name] = officePreset;
    
    qDebug() << "[NetworkPresetManager] ✅ Created default presets";
}

void NetworkPresetManager::createDefaultGroups()
{
    // Only create defaults if no groups exist
    if (!m_groups.isEmpty()) {
        return;
    }
    
    NetworkGroup homeGroup;
    homeGroup.name = "Home Networks";
    homeGroup.description = "Personal and home network configurations";
    homeGroup.color = "#4CAF50";
    m_groups[homeGroup.name] = homeGroup;
    
    NetworkGroup workGroup;
    workGroup.name = "Work Networks";
    workGroup.description = "Office and professional network configurations";
    workGroup.color = "#2196F3";
    m_groups[workGroup.name] = workGroup;
    
    qDebug() << "[NetworkPresetManager] ✅ Created default groups";
}

void NetworkPresetManager::startAutoSaveTimer()
{
    if (!m_autoSaveTimer) {
        m_autoSaveTimer = new QTimer(this);
        connect(m_autoSaveTimer, &QTimer::timeout, this, &NetworkPresetManager::autoSave);
    }
    
    if (m_autoSaveEnabled && m_autoSaveInterval > 0) {
        m_autoSaveTimer->start(m_autoSaveInterval * 1000);
        qDebug() << "[NetworkPresetManager] ⏰ Auto-save timer started (" << m_autoSaveInterval << "s)";
    }
}

void NetworkPresetManager::stopAutoSaveTimer()
{
    if (m_autoSaveTimer) {
        m_autoSaveTimer->stop();
        qDebug() << "[NetworkPresetManager] ⏰ Auto-save timer stopped";
    }
}

void NetworkPresetManager::autoSave()
{
    if (m_isDirty) {
        saveSettings();
    }
}

void NetworkPresetManager::updatePresetUsage(const QString &name)
{
    if (m_presets.contains(name)) {
        m_presets[name].usageCount++;
        m_presets[name].lastUsed = QDateTime::currentDateTime();
        
        // Update recently used list
        m_recentlyUsed.removeAll(name);
        m_recentlyUsed.prepend(name);
        
        // Limit recently used list
        while (m_recentlyUsed.size() > MAX_RECENT_COUNT) {
            m_recentlyUsed.removeLast();
        }
        
        m_isDirty = true;
    }
}

QString NetworkPresetManager::getConfigFilePath() const
{
    return m_configFilePath;
}

void NetworkPresetManager::setConfigFilePath(const QString &path)
{
    if (m_configFilePath != path) {
        if (m_isDirty) {
            saveSettings();
        }
        
        m_configFilePath = path;
        delete m_settings;
        m_settings = new QSettings(m_configFilePath, QSettings::IniFormat, this);
        
        loadSettings();
        
        qDebug() << "[NetworkPresetManager] 📁 Config path changed to:" << path;
    }
}

bool NetworkPresetManager::isAutoSaveEnabled() const
{
    return m_autoSaveEnabled;
}

void NetworkPresetManager::setAutoSaveEnabled(bool enabled)
{
    if (m_autoSaveEnabled != enabled) {
        m_autoSaveEnabled = enabled;
        m_isDirty = true;
        
        if (enabled) {
            startAutoSaveTimer();
        } else {
            stopAutoSaveTimer();
        }
        
        qDebug() << "[NetworkPresetManager] ⏰ Auto-save" << (enabled ? "enabled" : "disabled");
    }
}

int NetworkPresetManager::getAutoSaveInterval() const
{
    return m_autoSaveInterval;
}

void NetworkPresetManager::setAutoSaveInterval(int seconds)
{
    if (m_autoSaveInterval != seconds && seconds > 0) {
        m_autoSaveInterval = seconds;
        m_isDirty = true;
        
        if (m_autoSaveEnabled) {
            startAutoSaveTimer();
        }
        
        qDebug() << "[NetworkPresetManager] ⏰ Auto-save interval changed to" << seconds << "seconds";
    }
}

void NetworkPresetManager::onPresetUsed(const QString &presetName)
{
    std::cout << "[NetworkPresetManager] 📊 Preset used: " << presetName.toStdString() << std::endl;
    
    if (m_presets.contains(presetName)) {
        m_presets[presetName].lastUsed = QDateTime::currentDateTime();
        m_presets[presetName].usageCount++;
        
        // Update recently used list
        m_recentlyUsed.removeAll(presetName);
        m_recentlyUsed.prepend(presetName);
        
        // Limit recently used list
        while (m_recentlyUsed.size() > MAX_RECENT_COUNT) {
            m_recentlyUsed.removeLast();
        }
        
        m_isDirty = true;
        
        emit presetUpdated(presetName);
        
        // Auto-save after usage update
        saveSettings();
    }
}
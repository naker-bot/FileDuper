#ifndef THEMESYSTEM_H
#define THEMESYSTEM_H

#include <QString>
#include <QMap>
#include <QColor>
#include <QObject>
#include <QSettings>

class ThemeSystem : public QObject {
    Q_OBJECT

public:
    enum ThemeType {
        DARK_MODERN,
        LIGHT_CLEAN,
        BLUE_GRADIENT,
        CYBERPUNK_NEON,
        FOREST_GREEN,
        SUNSET_ORANGE,
        TWILIGHT_PURPLE,
        MINIMALIST_GREY,
        MATERIAL_DESIGN,
        RETRO_TERMINAL,
        NORD_THEME,
        DRACULA_THEME
    };

    struct ThemeColors {
        QString name;
        QString description;
        QColor primaryColor;
        QColor secondaryColor;
        QColor accentColor;
        QColor backgroundColor;
        QColor textColor;
        QColor borderColor;
        QColor buttonHoverColor;
        QColor successColor;
        QColor warningColor;
        QColor errorColor;
        QString styleName;
    };

    ThemeSystem(QObject *parent = nullptr);
    ~ThemeSystem();

    // Theme-Verwaltung
    QString getCurrentTheme() const;
    void setCurrentTheme(ThemeType type);
    void setCurrentThemeByName(const QString &themeName);
    
    // Theme-Abruf
    ThemeColors getTheme(ThemeType type) const;
    ThemeColors getCurrentThemeColors() const;
    QStringList getAllThemeNames() const;
    
    // Stylesheet generieren
    QString generateStylesheet(ThemeType type) const;
    QString getCurrentStylesheet() const;
    
    // Speichern/Laden
    void saveCurrentTheme();
    void loadSavedTheme();
    
    // Theme-Details
    QString getThemeName(ThemeType type) const;
    QString getThemeDescription(ThemeType type) const;

private:
    // Theme-Definitionen
    ThemeColors createDarkModernTheme() const;
    ThemeColors createLightCleanTheme() const;
    ThemeColors createBlueGradientTheme() const;
    ThemeColors createCyberpunkNeonTheme() const;
    ThemeColors createForestGreenTheme() const;
    ThemeColors createSunsetOrangeTheme() const;
    ThemeColors createTwilightPurpleTheme() const;
    ThemeColors createMinimalistGreyTheme() const;
    ThemeColors createMaterialDesignTheme() const;
    ThemeColors createRetroTerminalTheme() const;
    ThemeColors createNordTheme() const;
    ThemeColors createDraculaTheme() const;

    // Stylesheet-Generierung
    QString buildStylesheet(const ThemeColors &colors) const;

    QMap<ThemeType, ThemeColors> m_themes;
    ThemeType m_currentTheme;
    QSettings m_settings;
};

#endif // THEMESYSTEM_H

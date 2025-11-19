#include "themesystem.h"
#include <QApplication>
#include <QDebug>

ThemeSystem::ThemeSystem(QObject *parent)
    : QObject(parent)
    , m_currentTheme(DARK_MODERN)
    , m_settings("FileDuper", "FileDuper", parent)
{
    qDebug() << "[ThemeSystem] 🎨 Theme-System initialisiert";
    
    // Themes initialisieren
    m_themes[DARK_MODERN] = createDarkModernTheme();
    m_themes[LIGHT_CLEAN] = createLightCleanTheme();
    m_themes[BLUE_GRADIENT] = createBlueGradientTheme();
    m_themes[CYBERPUNK_NEON] = createCyberpunkNeonTheme();
    m_themes[FOREST_GREEN] = createForestGreenTheme();
    m_themes[SUNSET_ORANGE] = createSunsetOrangeTheme();
    m_themes[TWILIGHT_PURPLE] = createTwilightPurpleTheme();
    m_themes[MINIMALIST_GREY] = createMinimalistGreyTheme();
    m_themes[MATERIAL_DESIGN] = createMaterialDesignTheme();
    m_themes[RETRO_TERMINAL] = createRetroTerminalTheme();
    m_themes[NORD_THEME] = createNordTheme();
    m_themes[DRACULA_THEME] = createDraculaTheme();
    
    // Gespeichertes Theme laden
    loadSavedTheme();
}

ThemeSystem::~ThemeSystem()
{
}

QString ThemeSystem::getCurrentTheme() const
{
    return getThemeName(m_currentTheme);
}

void ThemeSystem::setCurrentTheme(ThemeType type)
{
    m_currentTheme = type;
    qDebug() << "[ThemeSystem] 🎨 Theme gewechselt zu:" << getThemeName(type);
}

void ThemeSystem::setCurrentThemeByName(const QString &themeName)
{
    for (auto it = m_themes.begin(); it != m_themes.end(); ++it) {
        if (it.value().name == themeName) {
            setCurrentTheme(it.key());
            return;
        }
    }
    qWarning() << "[ThemeSystem] ❌ Theme nicht gefunden:" << themeName;
}

ThemeSystem::ThemeColors ThemeSystem::getTheme(ThemeType type) const
{
    return m_themes.value(type);
}

ThemeSystem::ThemeColors ThemeSystem::getCurrentThemeColors() const
{
    return m_themes.value(m_currentTheme);
}

QStringList ThemeSystem::getAllThemeNames() const
{
    QStringList names;
    for (const auto &theme : m_themes) {
        names << theme.name;
    }
    return names;
}

QString ThemeSystem::generateStylesheet(ThemeType type) const
{
    return buildStylesheet(m_themes.value(type));
}

QString ThemeSystem::getCurrentStylesheet() const
{
    return buildStylesheet(getCurrentThemeColors());
}

void ThemeSystem::saveCurrentTheme()
{
    m_settings.setValue("Theme", getThemeName(m_currentTheme));
    m_settings.sync();
    qDebug() << "[ThemeSystem] 💾 Theme gespeichert:" << getThemeName(m_currentTheme);
}

void ThemeSystem::loadSavedTheme()
{
    QString savedTheme = m_settings.value("Theme", "Dark Modern").toString();
    setCurrentThemeByName(savedTheme);
    qDebug() << "[ThemeSystem] 📂 Theme geladen:" << savedTheme;
}

QString ThemeSystem::getThemeName(ThemeType type) const
{
    return m_themes.value(type).name;
}

QString ThemeSystem::getThemeDescription(ThemeType type) const
{
    return m_themes.value(type).description;
}

// ===== THEME DEFINITIONEN =====

ThemeSystem::ThemeColors ThemeSystem::createDarkModernTheme() const
{
    ThemeColors theme;
    theme.name = "Dark Modern";
    theme.description = "Modernes dunkles Design mit glattem Gradient";
    theme.primaryColor = QColor("#1e1e1e");
    theme.secondaryColor = QColor("#2d2d2d");
    theme.accentColor = QColor("#0d7fd9");
    theme.backgroundColor = QColor("#1e1e1e");
    theme.textColor = QColor("#ffffff");
    theme.borderColor = QColor("#3d3d3d");
    theme.buttonHoverColor = QColor("#0a5fa3");
    theme.successColor = QColor("#10b981");
    theme.warningColor = QColor("#f59e0b");
    theme.errorColor = QColor("#ef4444");
    theme.styleName = "dark_modern";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createLightCleanTheme() const
{
    ThemeColors theme;
    theme.name = "Light Clean";
    theme.description = "Helles, sauberes Design für minimalistisches Arbeiten";
    theme.primaryColor = QColor("#ffffff");
    theme.secondaryColor = QColor("#f5f5f5");
    theme.accentColor = QColor("#2563eb");
    theme.backgroundColor = QColor("#ffffff");
    theme.textColor = QColor("#1f2937");
    theme.borderColor = QColor("#e5e7eb");
    theme.buttonHoverColor = QColor("#1d4ed8");
    theme.successColor = QColor("#059669");
    theme.warningColor = QColor("#d97706");
    theme.errorColor = QColor("#dc2626");
    theme.styleName = "light_clean";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createBlueGradientTheme() const
{
    ThemeColors theme;
    theme.name = "Blue Gradient";
    theme.description = "Professionelles Design mit blauem Gradient";
    theme.primaryColor = QColor("#001f3f");
    theme.secondaryColor = QColor("#003d7a");
    theme.accentColor = QColor("#00d4ff");
    theme.backgroundColor = QColor("#001f3f");
    theme.textColor = QColor("#e0f7ff");
    theme.borderColor = QColor("#0066cc");
    theme.buttonHoverColor = QColor("#00a8cc");
    theme.successColor = QColor("#00c853");
    theme.warningColor = QColor("#ffb300");
    theme.errorColor = QColor("#ff5252");
    theme.styleName = "blue_gradient";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createCyberpunkNeonTheme() const
{
    ThemeColors theme;
    theme.name = "Cyberpunk Neon";
    theme.description = "Futuristisches Neon-Design mit Cyberpunk-Vibes";
    theme.primaryColor = QColor("#0d0221");
    theme.secondaryColor = QColor("#1a0033");
    theme.accentColor = QColor("#ff006e");
    theme.backgroundColor = QColor("#0d0221");
    theme.textColor = QColor("#00d9ff");
    theme.borderColor = QColor("#ff006e");
    theme.buttonHoverColor = QColor("#c2185b");
    theme.successColor = QColor("#76ff03");
    theme.warningColor = QColor("#ffab00");
    theme.errorColor = QColor("#ff5252");
    theme.styleName = "cyberpunk_neon";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createForestGreenTheme() const
{
    ThemeColors theme;
    theme.name = "Forest Green";
    theme.description = "Naturbelassenes Design mit Waldgrün-Tönen";
    theme.primaryColor = QColor("#1b3a2e");
    theme.secondaryColor = QColor("#2d5a4a");
    theme.accentColor = QColor("#52b788");
    theme.backgroundColor = QColor("#1b3a2e");
    theme.textColor = QColor("#d4f1d4");
    theme.borderColor = QColor("#2d5a4a");
    theme.buttonHoverColor = QColor("#40916c");
    theme.successColor = QColor("#52b788");
    theme.warningColor = QColor("#fdb833");
    theme.errorColor = QColor("#e76f51");
    theme.styleName = "forest_green";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createSunsetOrangeTheme() const
{
    ThemeColors theme;
    theme.name = "Sunset Orange";
    theme.description = "Warmes Design inspiriert von Sonnenuntergängen";
    theme.primaryColor = QColor("#2d1b16");
    theme.secondaryColor = QColor("#5c2e1f");
    theme.accentColor = QColor("#ff7a3d");
    theme.backgroundColor = QColor("#2d1b16");
    theme.textColor = QColor("#ffe6d5");
    theme.borderColor = QColor("#8b4513");
    theme.buttonHoverColor = QColor("#e85e2c");
    theme.successColor = QColor("#66bb6a");
    theme.warningColor = QColor("#ffa726");
    theme.errorColor = QColor("#ff7043");
    theme.styleName = "sunset_orange";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createTwilightPurpleTheme() const
{
    ThemeColors theme;
    theme.name = "Twilight Purple";
    theme.description = "Geheimnisvolles Design mit Lila-Tönen";
    theme.primaryColor = QColor("#2d1b4e");
    theme.secondaryColor = QColor("#44236d");
    theme.accentColor = QColor("#b367d8");
    theme.backgroundColor = QColor("#2d1b4e");
    theme.textColor = QColor("#e5d5ff");
    theme.borderColor = QColor("#664499");
    theme.buttonHoverColor = QColor("#9d4edd");
    theme.successColor = QColor("#66bb6a");
    theme.warningColor = QColor("#ffa726");
    theme.errorColor = QColor("#ef5350");
    theme.styleName = "twilight_purple";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createMinimalistGreyTheme() const
{
    ThemeColors theme;
    theme.name = "Minimalist Grey";
    theme.description = "Minimalistisches Design in Grautönen";
    theme.primaryColor = QColor("#2a2a2a");
    theme.secondaryColor = QColor("#3f3f3f");
    theme.accentColor = QColor("#757575");
    theme.backgroundColor = QColor("#2a2a2a");
    theme.textColor = QColor("#e0e0e0");
    theme.borderColor = QColor("#424242");
    theme.buttonHoverColor = QColor("#616161");
    theme.successColor = QColor("#81c784");
    theme.warningColor = QColor("#ffb74d");
    theme.errorColor = QColor("#e57373");
    theme.styleName = "minimalist_grey";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createMaterialDesignTheme() const
{
    ThemeColors theme;
    theme.name = "Material Design";
    theme.description = "Google Material Design 3 Stil";
    theme.primaryColor = QColor("#1f1f1f");
    theme.secondaryColor = QColor("#2f2f2f");
    theme.accentColor = QColor("#6750a4");
    theme.backgroundColor = QColor("#1f1f1f");
    theme.textColor = QColor("#e1e1e6");
    theme.borderColor = QColor("#3f3f3f");
    theme.buttonHoverColor = QColor("#5a439a");
    theme.successColor = QColor("#81c784");
    theme.warningColor = QColor("#ffb74d");
    theme.errorColor = QColor("#f44336");
    theme.styleName = "material_design";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createRetroTerminalTheme() const
{
    ThemeColors theme;
    theme.name = "Retro Terminal";
    theme.description = "Klassisches Retro-Terminal-Design wie in den 80ern";
    theme.primaryColor = QColor("#000000");
    theme.secondaryColor = QColor("#1a1a1a");
    theme.accentColor = QColor("#00ff00");
    theme.backgroundColor = QColor("#000000");
    theme.textColor = QColor("#00ff00");
    theme.borderColor = QColor("#00aa00");
    theme.buttonHoverColor = QColor("#00dd00");
    theme.successColor = QColor("#00ff00");
    theme.warningColor = QColor("#ffff00");
    theme.errorColor = QColor("#ff0000");
    theme.styleName = "retro_terminal";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createNordTheme() const
{
    ThemeColors theme;
    theme.name = "Nord";
    theme.description = "Arctisches Farb-Palette Design";
    theme.primaryColor = QColor("#2e3440");
    theme.secondaryColor = QColor("#3b4252");
    theme.accentColor = QColor("#88c0d0");
    theme.backgroundColor = QColor("#2e3440");
    theme.textColor = QColor("#eceff4");
    theme.borderColor = QColor("#4c566a");
    theme.buttonHoverColor = QColor("#81a1c1");
    theme.successColor = QColor("#a3be8c");
    theme.warningColor = QColor("#ebcb8b");
    theme.errorColor = QColor("#bf616a");
    theme.styleName = "nord";
    return theme;
}

ThemeSystem::ThemeColors ThemeSystem::createDraculaTheme() const
{
    ThemeColors theme;
    theme.name = "Dracula";
    theme.description = "Dunkles, vampirisches Design";
    theme.primaryColor = QColor("#282a36");
    theme.secondaryColor = QColor("#44475a");
    theme.accentColor = QColor("#ff79c6");
    theme.backgroundColor = QColor("#282a36");
    theme.textColor = QColor("#f8f8f2");
    theme.borderColor = QColor("#44475a");
    theme.buttonHoverColor = QColor("#bd93f9");
    theme.successColor = QColor("#50fa7b");
    theme.warningColor = QColor("#f1fa8c");
    theme.errorColor = QColor("#ff5555");
    theme.styleName = "dracula";
    return theme;
}

// ===== STYLESHEET GENERIERUNG =====

QString ThemeSystem::buildStylesheet(const ThemeColors &colors) const
{
    return QString(R"(
        /* === GLOBAL STYLES === */
        QWidget {
            background-color: %1;
            color: %2;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 10pt;
        }

        /* === MAIN WINDOW === */
        QMainWindow {
            background-color: %1;
        }

        /* === DIALOGS === */
        QDialog, QMessageBox {
            background-color: %1;
            border: 1px solid %4;
        }

        /* === BUTTONS === */
        QPushButton {
            background-color: %3;
            color: white;
            border: none;
            border-radius: 4px;
            padding: 6px 12px;
            font-weight: bold;
        }

        QPushButton:hover {
            background-color: %7;
            border: 1px solid %3;
        }

        QPushButton:pressed {
            background-color: %3;
            padding: 7px 11px;
        }

        QPushButton:disabled {
            background-color: %4;
            color: %2;
            opacity: 0.5;
        }

        /* === INPUT FIELDS === */
        QLineEdit, QTextEdit, QPlainTextEdit, QSpinBox, QDoubleSpinBox, QComboBox {
            background-color: %5;
            color: %2;
            border: 1px solid %4;
            border-radius: 3px;
            padding: 4px;
            selection-background-color: %3;
        }

        QLineEdit:focus, QTextEdit:focus, QPlainTextEdit:focus {
            border: 2px solid %3;
        }

        /* === LABELS === */
        QLabel {
            color: %2;
            background-color: transparent;
        }

        /* === TABS === */
        QTabWidget::pane {
            border: 1px solid %4;
        }

        QTabBar::tab {
            background-color: %5;
            color: %2;
            padding: 6px 12px;
            border: 1px solid %4;
        }

        QTabBar::tab:selected {
            background-color: %3;
            color: white;
        }

        QTabBar::tab:hover {
            background-color: %5;
        }

        /* === MENUS === */
        QMenuBar {
            background-color: %5;
            color: %2;
            border-bottom: 1px solid %4;
        }

        QMenuBar::item:selected {
            background-color: %3;
            color: white;
        }

        QMenu {
            background-color: %5;
            color: %2;
            border: 1px solid %4;
        }

        QMenu::item:selected {
            background-color: %3;
            color: white;
        }

        /* === TOOLBARS === */
        QToolBar {
            background-color: %5;
            border: 1px solid %4;
            spacing: 3px;
        }

        /* === SCROLLBARS === */
        QScrollBar:vertical {
            background-color: %5;
            width: 14px;
        }

        QScrollBar::handle:vertical {
            background-color: %4;
            border-radius: 7px;
            min-height: 20px;
        }

        QScrollBar::handle:vertical:hover {
            background-color: %3;
        }

        QScrollBar:horizontal {
            background-color: %5;
            height: 14px;
        }

        QScrollBar::handle:horizontal {
            background-color: %4;
            border-radius: 7px;
            min-width: 20px;
        }

        QScrollBar::handle:horizontal:hover {
            background-color: %3;
        }

        /* === TREES & TABLES === */
        QTreeWidget, QListWidget, QTableWidget {
            background-color: %5;
            color: %2;
            border: 1px solid %4;
            gridline-color: %4;
            alternate-background-color: %1;
        }

        QTreeWidget::item:selected, QListWidget::item:selected, QTableWidget::item:selected {
            background-color: %3;
            color: white;
        }

        QHeaderView::section {
            background-color: %5;
            color: %2;
            padding: 4px;
            border: 1px solid %4;
        }

        /* === PROGRESS BARS === */
        QProgressBar {
            background-color: %5;
            border: 1px solid %4;
            border-radius: 3px;
            height: 18px;
        }

        QProgressBar::chunk {
            background-color: %3;
        }

        /* === SLIDERS === */
        QSlider::groove:horizontal {
            background-color: %5;
            border: 1px solid %4;
            height: 6px;
        }

        QSlider::handle:horizontal {
            background-color: %3;
            width: 14px;
            margin: -4px 0;
            border-radius: 7px;
        }

        QSlider::handle:horizontal:hover {
            background-color: %7;
        }

        /* === STATUS BAR === */
        QStatusBar {
            background-color: %5;
            color: %2;
            border-top: 1px solid %4;
        }

        /* === DOCK WIDGETS === */
        QDockWidget {
            color: %2;
            border: 1px solid %4;
        }

        QDockWidget::title {
            background-color: %5;
            padding: 3px;
        }

        /* === CHECKBOX & RADIO === */
        QCheckBox, QRadioButton {
            color: %2;
            spacing: 5px;
        }

        QCheckBox::indicator, QRadioButton::indicator {
            width: 16px;
            height: 16px;
            border: 1px solid %4;
            border-radius: 3px;
            background-color: %5;
        }

        QCheckBox::indicator:checked, QRadioButton::indicator:checked {
            background-color: %3;
            border: 1px solid %3;
        }

        /* === GROUP BOX === */
        QGroupBox {
            color: %2;
            border: 1px solid %4;
            border-radius: 4px;
            margin-top: 8px;
            padding-top: 8px;
        }

        QGroupBox::title {
            subcontrol-origin: margin;
            left: 10px;
            padding: 0 3px 0 3px;
        }

        /* === SPINNER === */
        QSpinBox::up-button, QSpinBox::down-button,
        QDoubleSpinBox::up-button, QDoubleSpinBox::down-button {
            background-color: %3;
            width: 16px;
        }

        QSpinBox::up-button:hover, QSpinBox::down-button:hover,
        QDoubleSpinBox::up-button:hover, QDoubleSpinBox::down-button:hover {
            background-color: %7;
        }
    )")
    .arg(colors.backgroundColor.name())      // %1 - Background
    .arg(colors.textColor.name())             // %2 - Text
    .arg(colors.accentColor.name())           // %3 - Accent
    .arg(colors.borderColor.name())           // %4 - Border
    .arg(colors.secondaryColor.name())        // %5 - Secondary
    .arg(colors.primaryColor.name())          // %6 - Primary
    .arg(colors.buttonHoverColor.name());     // %7 - Hover
}

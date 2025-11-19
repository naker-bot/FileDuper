#include "thememanagerwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QApplication>
#include <QDebug>

ThemeManagerWidget::ThemeManagerWidget(QWidget *parent)
    : QWidget(parent)
    , m_themeSystem(nullptr)
{
    setupUI();
}

ThemeManagerWidget::~ThemeManagerWidget()
{
}

void ThemeManagerWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);

    // Title
    QLabel *titleLabel = new QLabel("🎨 Theme Manager");
    QFont titleFont = titleLabel->font();
    titleFont.setPointSize(12);
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    mainLayout->addWidget(titleLabel);

    // Theme Selection Group
    QGroupBox *selectionGroup = new QGroupBox("Theme Selection");
    QVBoxLayout *selectionLayout = new QVBoxLayout(selectionGroup);

    QHBoxLayout *comboLayout = new QHBoxLayout();
    QLabel *themeLabel = new QLabel("Available Themes:");
    m_themeCombo = new QComboBox();
    comboLayout->addWidget(themeLabel);
    comboLayout->addWidget(m_themeCombo, 1);
    selectionLayout->addLayout(comboLayout);

    m_descriptionLabel = new QLabel("Select a theme to see its description");
    m_descriptionLabel->setWordWrap(true);
    m_descriptionLabel->setStyleSheet("color: gray; font-style: italic;");
    selectionLayout->addWidget(m_descriptionLabel);

    mainLayout->addWidget(selectionGroup);

    // Preview Group
    QGroupBox *previewGroup = new QGroupBox("Preview");
    QVBoxLayout *previewLayout = new QVBoxLayout(previewGroup);

    m_previewLabel = new QLabel();
    m_previewLabel->setMinimumHeight(100);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setText("Theme preview will be shown here");
    previewLayout->addWidget(m_previewLabel);

    mainLayout->addWidget(previewGroup);

    // Buttons Group
    QGroupBox *buttonsGroup = new QGroupBox("Actions");
    QHBoxLayout *buttonsLayout = new QHBoxLayout(buttonsGroup);

    m_previewButton = new QPushButton("👁️ Preview");
    m_applyButton = new QPushButton("✓ Apply Theme");
    m_resetButton = new QPushButton("↻ Reset to Default");

    m_previewButton->setMinimumWidth(100);
    m_applyButton->setMinimumWidth(100);
    m_resetButton->setMinimumWidth(100);

    buttonsLayout->addWidget(m_previewButton);
    buttonsLayout->addWidget(m_applyButton);
    buttonsLayout->addWidget(m_resetButton);
    buttonsLayout->addStretch();

    mainLayout->addWidget(buttonsGroup);
    mainLayout->addStretch();

    // Connect signals
    connect(m_themeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ThemeManagerWidget::onThemeChanged);
    connect(m_applyButton, &QPushButton::clicked,
            this, &ThemeManagerWidget::onApplyTheme);
    connect(m_previewButton, &QPushButton::clicked,
            this, &ThemeManagerWidget::onPreviewTheme);
    connect(m_resetButton, &QPushButton::clicked,
            this, &ThemeManagerWidget::onResetTheme);

    setMinimumWidth(350);
}

void ThemeManagerWidget::setThemeSystem(ThemeSystem *themeSystem)
{
    m_themeSystem = themeSystem;
    updateThemeList();
}

void ThemeManagerWidget::updateThemeList()
{
    if (!m_themeSystem) return;

    m_themeCombo->clear();
    QStringList themes = m_themeSystem->getAllThemeNames();
    m_themeCombo->addItems(themes);

    // Select current theme
    QString current = m_themeSystem->getCurrentTheme();
    int index = m_themeCombo->findText(current);
    if (index >= 0) {
        m_themeCombo->setCurrentIndex(index);
    }
}

void ThemeManagerWidget::onThemeChanged(int index)
{
    if (!m_themeSystem || index < 0) return;

    QString themeName = m_themeCombo->currentText();
    
    // Find and display description
    ThemeSystem::ThemeColors colors = m_themeSystem->getTheme(
        static_cast<ThemeSystem::ThemeType>(index));
    m_descriptionLabel->setText("📝 " + colors.description);

    // Show color preview
    QString previewHTML = QString(
        "<table cellpadding='8' cellspacing='2' width='100%%'>"
        "<tr><td>Primary:</td><td style='background-color: %1; width: 50px; height: 20px;'></td></tr>"
        "<tr><td>Accent:</td><td style='background-color: %2; width: 50px; height: 20px;'></td></tr>"
        "<tr><td>Success:</td><td style='background-color: %3; width: 50px; height: 20px;'></td></tr>"
        "<tr><td>Warning:</td><td style='background-color: %4; width: 50px; height: 20px;'></td></tr>"
        "<tr><td>Error:</td><td style='background-color: %5; width: 50px; height: 20px;'></td></tr>"
        "</table>")
        .arg(colors.primaryColor.name())
        .arg(colors.accentColor.name())
        .arg(colors.successColor.name())
        .arg(colors.warningColor.name())
        .arg(colors.errorColor.name());

    m_previewLabel->setText(previewHTML);

    qDebug() << "[ThemeManager] 🎨 Theme ausgewählt:" << themeName;
}

void ThemeManagerWidget::onApplyTheme()
{
    if (!m_themeSystem) return;

    QString themeName = m_themeCombo->currentText();
    m_themeSystem->setCurrentThemeByName(themeName);
    
    // Apply stylesheet to entire application
    QString stylesheet = m_themeSystem->getCurrentStylesheet();
    qApp->setStyleSheet(stylesheet);
    
    // Save theme preference
    m_themeSystem->saveCurrentTheme();

    qDebug() << "[ThemeManager] ✓ Theme angewendet:" << themeName;
}

void ThemeManagerWidget::onPreviewTheme()
{
    if (!m_themeSystem) return;

    QString themeName = m_themeCombo->currentText();
    
    // Apply stylesheet to preview
    QString stylesheet = m_themeSystem->generateStylesheet(
        static_cast<ThemeSystem::ThemeType>(m_themeCombo->currentIndex()));
    
    // Create temporary preview window
    QWidget *previewWindow = new QWidget();
    previewWindow->setWindowTitle("Theme Preview - " + themeName);
    previewWindow->setStyleSheet(stylesheet);
    previewWindow->setMinimumSize(400, 300);
    previewWindow->show();

    qDebug() << "[ThemeManager] 👁️ Theme Vorschau:" << themeName;
}

void ThemeManagerWidget::onResetTheme()
{
    if (!m_themeSystem) return;

    m_themeSystem->setCurrentTheme(ThemeSystem::DARK_MODERN);
    QString stylesheet = m_themeSystem->getCurrentStylesheet();
    qApp->setStyleSheet(stylesheet);
    m_themeSystem->saveCurrentTheme();
    
    updateThemeList();

    qDebug() << "[ThemeManager] ↻ Theme zurückgesetzt zu Dark Modern";
}

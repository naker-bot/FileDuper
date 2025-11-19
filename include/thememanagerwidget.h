#ifndef THEMEMANAGERWIDGET_H
#define THEMEMANAGERWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include "themesystem.h"

class ThemeManagerWidget : public QWidget {
    Q_OBJECT

public:
    explicit ThemeManagerWidget(QWidget *parent = nullptr);
    ~ThemeManagerWidget();

    void setThemeSystem(ThemeSystem *themeSystem);

private slots:
    void onThemeChanged(int index);
    void onApplyTheme();
    void onPreviewTheme();
    void onResetTheme();

private:
    void setupUI();
    void updateThemeList();
    void applyTheme(const QString &themeName);

    ThemeSystem *m_themeSystem;
    QComboBox *m_themeCombo;
    QPushButton *m_applyButton;
    QPushButton *m_previewButton;
    QPushButton *m_resetButton;
    QLabel *m_descriptionLabel;
    QLabel *m_previewLabel;
};

#endif // THEMEMANAGERWIDGET_H

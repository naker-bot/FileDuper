#ifndef NETWORKRANGEWIDGET_H
#define NETWORKRANGEWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QTextEdit>
#include <QDialog>
#include <QSettings>
#include "networkscanner.h"

class NetworkRangeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkRangeDialog(QWidget *parent = nullptr);
    
    QString getRangeName() const;
    QString getRangeCidr() const;
    QString getRangeDescription() const;
    
    void setRangeName(const QString &name);
    void setRangeCidr(const QString &cidr);
    void setRangeDescription(const QString &description);

private slots:
    void validateInput();

private:
    QLineEdit *m_nameEdit;
    QLineEdit *m_cidrEdit;
    QTextEdit *m_descriptionEdit;
    QPushButton *m_okButton;
    QPushButton *m_cancelButton;
    QLabel *m_statusLabel;
};

class NetworkRangeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkRangeWidget(QWidget *parent = nullptr);
    
    void setNetworkScanner(NetworkScanner *scanner);
    void refreshRanges();
    
    // 🌐 Methods for MainWindow integration
    QStringList getSelectedRanges() const;
    QStringList getCustomRanges() const;
    void setAutoDetectedRanges(const QStringList &ranges);
    
    // Settings persistence
    void saveSettings();
    void loadSettings();

signals:
    void rangeChanged(const QString &rangeName);
    void customRangeAdded(const QString &range, const QString &description);
    void rangeSelected(const QString &range);
    void rangesLoaded(int customRangeCount); // Signal when ranges are loaded

private slots:
    void onRangeSelectionChanged();
    void onAddCustomRange();
    void onRemoveRange();
    void onRefreshRanges();
    void onNetworkRangeDetected(const NetworkRange &range);
    void updateRangeList();

private:
    void setupUI();
    void updateRangeInfo();
    
    NetworkScanner *m_networkScanner;
    
    // UI Components
    QComboBox *m_rangeCombo;
    QPushButton *m_addButton;
    QPushButton *m_removeButton;
    QPushButton *m_refreshButton;
    QLabel *m_rangeInfoLabel;
    QListWidget *m_rangeListWidget;
    
    // Current selection
    QString m_currentRange;
};

#endif // NETWORKRANGEWIDGET_H

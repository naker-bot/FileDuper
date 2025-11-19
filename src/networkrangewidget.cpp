#include "networkrangewidget.h"
#include <QGridLayout>
#include <QMessageBox>
#include <QRegularExpression>
#include <QSettings>
#include <QDebug>

// NetworkRangeDialog Implementation
NetworkRangeDialog::NetworkRangeDialog(QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle("Add Custom Network Range");
    setModal(true);
    resize(400, 300);
    
    QVBoxLayout *layout = new QVBoxLayout(this);
    
    // Name input
    layout->addWidget(new QLabel("Range Name:"));
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("e.g., 'Office Network' or 'Home Lab'");
    layout->addWidget(m_nameEdit);
    
    // CIDR input
    layout->addWidget(new QLabel("Network CIDR:"));
    m_cidrEdit = new QLineEdit(this);
    m_cidrEdit->setPlaceholderText("e.g., 192.168.30.0/24 or 10.0.0.0/16");
    layout->addWidget(m_cidrEdit);
    
    // Description input
    layout->addWidget(new QLabel("Description (optional):"));
    m_descriptionEdit = new QTextEdit(this);
    m_descriptionEdit->setMaximumHeight(60);
    m_descriptionEdit->setPlaceholderText("Optional description for this network range");
    layout->addWidget(m_descriptionEdit);
    
    // Status label
    m_statusLabel = new QLabel(this);
    m_statusLabel->setStyleSheet("QLabel { color: red; }");
    layout->addWidget(m_statusLabel);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    m_okButton = new QPushButton("Add Range", this);
    m_cancelButton = new QPushButton("Cancel", this);
    
    m_okButton->setEnabled(false);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(m_okButton);
    buttonLayout->addWidget(m_cancelButton);
    layout->addLayout(buttonLayout);
    
    // Connections
    connect(m_nameEdit, &QLineEdit::textChanged, this, &NetworkRangeDialog::validateInput);
    connect(m_cidrEdit, &QLineEdit::textChanged, this, &NetworkRangeDialog::validateInput);
    connect(m_okButton, &QPushButton::clicked, this, &QDialog::accept);
    connect(m_cancelButton, &QPushButton::clicked, this, &QDialog::reject);
    
    // Set focus to name field
    m_nameEdit->setFocus();
}

QString NetworkRangeDialog::getRangeName() const
{
    return m_nameEdit->text().trimmed();
}

QString NetworkRangeDialog::getRangeCidr() const
{
    return m_cidrEdit->text().trimmed();
}

QString NetworkRangeDialog::getRangeDescription() const
{
    return m_descriptionEdit->toPlainText().trimmed();
}

void NetworkRangeDialog::setRangeName(const QString &name)
{
    m_nameEdit->setText(name);
}

void NetworkRangeDialog::setRangeCidr(const QString &cidr)
{
    m_cidrEdit->setText(cidr);
}

void NetworkRangeDialog::setRangeDescription(const QString &description)
{
    m_descriptionEdit->setPlainText(description);
}

void NetworkRangeDialog::validateInput()
{
    QString name = getRangeName();
    QString cidr = getRangeCidr();
    
    bool valid = true;
    QString status;
    
    // Validate name
    if (name.isEmpty()) {
        valid = false;
        status = "Range name is required";
    }
    // Validate CIDR
    else if (cidr.isEmpty()) {
        valid = false;
        status = "CIDR notation is required";
    }
    else {
        // Validate CIDR format
        QRegularExpression cidrRegex(R"(^(\d{1,3}\.){3}\d{1,3}/\d{1,2}$)");
        if (!cidrRegex.match(cidr).hasMatch()) {
            valid = false;
            status = "Invalid CIDR format (use x.x.x.x/xx)";
        } else {
            QStringList parts = cidr.split('/');
            QStringList ipParts = parts[0].split('.');
            int prefix = parts[1].toInt();
            
            // Validate IP octets
            for (const QString &octet : ipParts) {
                int value = octet.toInt();
                if (value < 0 || value > 255) {
                    valid = false;
                    status = "Invalid IP address in CIDR";
                    break;
                }
            }
            
            // Validate prefix length
            if (valid && (prefix < 8 || prefix > 30)) {
                valid = false;
                status = "Prefix length should be between 8 and 30";
            }
            
            if (valid) {
                status = "✓ Valid network range";
                m_statusLabel->setStyleSheet("QLabel { color: green; }");
            }
        }
    }
    
    m_statusLabel->setText(status);
    if (!valid) {
        m_statusLabel->setStyleSheet("QLabel { color: red; }");
    }
    
    m_okButton->setEnabled(valid);
}

// NetworkRangeWidget Implementation
NetworkRangeWidget::NetworkRangeWidget(QWidget *parent)
    : QWidget(parent), m_networkScanner(nullptr)
{
    setupUI();
    // Note: loadSettings() will be called after setNetworkScanner()
}

void NetworkRangeWidget::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Title
    QLabel *titleLabel = new QLabel("Network Range Selection");
    titleLabel->setStyleSheet("QLabel { font-weight: bold; font-size: 12pt; }");
    mainLayout->addWidget(titleLabel);
    
    // Range selection combo
    QHBoxLayout *comboLayout = new QHBoxLayout();
    comboLayout->addWidget(new QLabel("Active Range:"));
    
    m_rangeCombo = new QComboBox(this);
    m_rangeCombo->setMinimumWidth(200);
    comboLayout->addWidget(m_rangeCombo);
    
    comboLayout->addStretch();
    mainLayout->addLayout(comboLayout);
    
    // Range info
    m_rangeInfoLabel = new QLabel("No range selected");
    m_rangeInfoLabel->setStyleSheet("QLabel { color: gray; font-style: italic; }");
    mainLayout->addWidget(m_rangeInfoLabel);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    m_addButton = new QPushButton("➕ Add Custom Range", this);
    m_removeButton = new QPushButton("➖ Remove Range", this);
    m_refreshButton = new QPushButton("🔄 Refresh Auto-Detection", this);
    
    buttonLayout->addWidget(m_addButton);
    buttonLayout->addWidget(m_removeButton);
    buttonLayout->addWidget(m_refreshButton);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    
    // Range list
    QGroupBox *listGroup = new QGroupBox("Available Network Ranges");
    QVBoxLayout *listLayout = new QVBoxLayout(listGroup);
    
    m_rangeListWidget = new QListWidget(this);
    m_rangeListWidget->setMaximumHeight(150);
    listLayout->addWidget(m_rangeListWidget);
    
    mainLayout->addWidget(listGroup);
    
    // Connections
    connect(m_rangeCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &NetworkRangeWidget::onRangeSelectionChanged);
    connect(m_addButton, &QPushButton::clicked, this, &NetworkRangeWidget::onAddCustomRange);
    connect(m_removeButton, &QPushButton::clicked, this, &NetworkRangeWidget::onRemoveRange);
    connect(m_refreshButton, &QPushButton::clicked, this, &NetworkRangeWidget::onRefreshRanges);
    
    // Initially disable remove button
    m_removeButton->setEnabled(false);
}

void NetworkRangeWidget::setNetworkScanner(NetworkScanner *scanner)
{
    m_networkScanner = scanner;
    
    if (m_networkScanner) {
        connect(m_networkScanner, &NetworkScanner::networkRangeDetected,
                this, &NetworkRangeWidget::onNetworkRangeDetected);
        connect(m_networkScanner, &NetworkScanner::networkRangeChanged,
                this, &NetworkRangeWidget::updateRangeList);
        
        // Load saved settings after scanner is set
        loadSettings();
        refreshRanges();
    }
}

void NetworkRangeWidget::refreshRanges()
{
    if (!m_networkScanner) return;
    
    QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
    QString activeRange = m_networkScanner->getActiveNetworkRange();
    
    // Update combo box
    m_rangeCombo->clear();
    for (const NetworkRange &range : ranges) {
        m_rangeCombo->addItem(QString("%1 (%2)").arg(range.name, range.cidr), range.name);
    }
    
    // Set active range
    for (int i = 0; i < m_rangeCombo->count(); ++i) {
        if (m_rangeCombo->itemData(i).toString() == activeRange) {
            m_rangeCombo->setCurrentIndex(i);
            break;
        }
    }
    
    updateRangeList();
    updateRangeInfo();
}

void NetworkRangeWidget::onRangeSelectionChanged()
{
    if (!m_networkScanner) return;
    
    QString selectedRange = m_rangeCombo->currentData().toString();
    if (!selectedRange.isEmpty()) {
        m_networkScanner->setActiveNetworkRange(selectedRange);
        m_currentRange = selectedRange;
        updateRangeInfo();
        emit rangeChanged(selectedRange);
        
        // Save current selection
        saveSettings();
        
        // 🌐 Emit signal for MainWindow integration
        emit rangeSelected(selectedRange);
    }
}

void NetworkRangeWidget::onAddCustomRange()
{
    NetworkRangeDialog dialog(this);
    
    // Pre-fill with common ranges
    if (dialog.exec() == QDialog::Accepted) {
        QString name = dialog.getRangeName();
        QString cidr = dialog.getRangeCidr();
        QString description = dialog.getRangeDescription();
        
        if (m_networkScanner) {
            m_networkScanner->addCustomNetworkRange(name, cidr, description);
            refreshRanges();
            
            // Save settings after adding custom range
            saveSettings();
            
            // 🌐 Emit signal for MainWindow integration
            emit customRangeAdded(cidr, description);
        }
    }
}

void NetworkRangeWidget::onRemoveRange()
{
    if (!m_networkScanner) return;
    
    QString currentRange = m_rangeCombo->currentData().toString();
    if (currentRange.isEmpty()) return;
    
    QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
    NetworkRange rangeToRemove;
    
    for (const NetworkRange &range : ranges) {
        if (range.name == currentRange) {
            rangeToRemove = range;
            break;
        }
    }
    
    if (rangeToRemove.isAutoDetected) {
        QMessageBox::information(this, "Cannot Remove", 
            "Auto-detected ranges cannot be removed. They will be recreated on next refresh.");
        return;
    }
    
    int ret = QMessageBox::question(this, "Remove Network Range",
        QString("Are you sure you want to remove the network range '%1' (%2)?")
        .arg(rangeToRemove.name, rangeToRemove.cidr),
        QMessageBox::Yes | QMessageBox::No);
    
    if (ret == QMessageBox::Yes) {
        m_networkScanner->removeNetworkRange(currentRange);
        refreshRanges();
        
        // Save settings after removing range
        saveSettings();
    }
}

void NetworkRangeWidget::onRefreshRanges()
{
    if (!m_networkScanner) return;
    
    m_networkScanner->autoDetectNetworkRanges();
    refreshRanges();
}

void NetworkRangeWidget::onNetworkRangeDetected(const NetworkRange &range)
{
    qDebug() << "Network range detected:" << range.name << range.cidr;
    refreshRanges();
}

void NetworkRangeWidget::updateRangeList()
{
    if (!m_networkScanner) return;
    
    m_rangeListWidget->clear();
    
    QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
    for (const NetworkRange &range : ranges) {
        QString itemText = QString("%1 (%2)")
            .arg(range.name, range.cidr);
        
        if (range.isAutoDetected) {
            itemText += " [Auto-detected]";
        }
        
        QListWidgetItem *item = new QListWidgetItem(itemText);
        
        // Color coding
        if (range.isAutoDetected) {
            item->setForeground(QColor(0, 150, 0)); // Green for auto-detected
        } else {
            item->setForeground(QColor(0, 0, 150)); // Blue for custom
        }
        
        if (range.name == m_currentRange) {
            item->setFont(QFont("", -1, QFont::Bold));
            item->setBackground(QColor(255, 255, 0, 50)); // Light yellow background
        }
        
        m_rangeListWidget->addItem(item);
    }
}

void NetworkRangeWidget::updateRangeInfo()
{
    if (!m_networkScanner) return;
    
    QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
    QString activeRange = m_networkScanner->getActiveNetworkRange();
    
    for (const NetworkRange &range : ranges) {
        if (range.name == activeRange) {
            QString info = QString("<b>%1</b><br>CIDR: %2<br>%3")
                .arg(range.name, range.cidr, range.description);
            
            if (range.isAutoDetected) {
                info += "<br><i>Auto-detected from network interface</i>";
            }
            
            m_rangeInfoLabel->setText(info);
            m_rangeInfoLabel->setStyleSheet("QLabel { color: black; }");
            
            // Enable/disable remove button
            m_removeButton->setEnabled(!range.isAutoDetected);
            return;
        }
    }
    
    m_rangeInfoLabel->setText("No range selected");
    m_rangeInfoLabel->setStyleSheet("QLabel { color: gray; font-style: italic; }");
    m_removeButton->setEnabled(false);
}

// 🌐 MainWindow Integration Methods
QStringList NetworkRangeWidget::getSelectedRanges() const
{
    QStringList selectedRanges;
    
    if (m_networkScanner) {
        QString activeRange = m_networkScanner->getActiveNetworkRange();
        if (!activeRange.isEmpty()) {
            selectedRanges.append(activeRange);
        }
    }
    
    return selectedRanges;
}

QStringList NetworkRangeWidget::getCustomRanges() const
{
    QStringList customRanges;
    
    if (m_networkScanner) {
        QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
        for (const NetworkRange &range : ranges) {
            if (!range.isAutoDetected) {
                customRanges.append(range.cidr);
            }
        }
    }
    
    return customRanges;
}

void NetworkRangeWidget::setAutoDetectedRanges(const QStringList &ranges)
{
    if (!m_networkScanner) return;
    
    // Auto-detected ranges are handled internally by NetworkScanner::autoDetectNetworkRanges()
    // We don't need to add them manually here - just refresh the display
    
    // Refresh the widget display to show the auto-detected ranges
    refreshRanges();
    updateRangeList();
}

void NetworkRangeWidget::saveSettings()
{
    QSettings settings("FileDuper", "NetworkRanges");
    
    settings.beginGroup("CustomRanges");
    settings.remove(""); // Clear existing entries
    
    if (m_networkScanner) {
        QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
        int customIndex = 0;
        
        for (const NetworkRange &range : ranges) {
            if (!range.isAutoDetected) { // Only save custom ranges
                settings.beginGroup(QString("Range_%1").arg(customIndex));
                settings.setValue("name", range.name);
                settings.setValue("cidr", range.cidr);
                settings.setValue("description", range.description);
                settings.endGroup();
                customIndex++;
            }
        }
        
        settings.setValue("count", customIndex);
    }
    
    settings.endGroup();
    
    // Save selected range
    settings.setValue("selectedRange", m_currentRange);
    
    qDebug() << "[NetworkRangeWidget] Settings saved";
    
    // Debug output - zeige was gespeichert wurde
    qDebug() << "[NetworkRangeWidget] Saved settings:";
    qDebug() << "  Selected range:" << m_currentRange;
    if (m_networkScanner) {
        QList<NetworkRange> ranges = m_networkScanner->getAvailableNetworkRanges();
        int customCount = 0;
        for (const auto &range : ranges) {
            if (!range.isAutoDetected) {
                qDebug() << "  Custom range" << customCount << ":" << range.name << range.cidr;
                customCount++;
            }
        }
        qDebug() << "  Total custom ranges:" << customCount;
    }
}

void NetworkRangeWidget::loadSettings()
{
    QSettings settings("FileDuper", "NetworkRanges");
    
    settings.beginGroup("CustomRanges");
    int count = settings.value("count", 0).toInt();
    
    qDebug() << "[NetworkRangeWidget] Loading settings, custom ranges count:" << count;
    
    if (m_networkScanner && count > 0) {
        for (int i = 0; i < count; ++i) {
            settings.beginGroup(QString("Range_%1").arg(i));
            
            QString name = settings.value("name").toString();
            QString cidr = settings.value("cidr").toString();
            QString description = settings.value("description").toString();
            
            qDebug() << "[NetworkRangeWidget] Loading range" << i << ":" << name << cidr << description;
            
            if (!name.isEmpty() && !cidr.isEmpty()) {
                // Add custom range to network scanner
                m_networkScanner->addCustomNetworkRange(name, cidr, description);
                
                qDebug() << "[NetworkRangeWidget] ✅ Loaded custom range:" << name << cidr;
            } else {
                qDebug() << "[NetworkRangeWidget] ❌ Skipped invalid range:" << name << cidr;
            }
            
            settings.endGroup();
        }
    }
    
    settings.endGroup();
    
    // Restore selected range
    m_currentRange = settings.value("selectedRange").toString();
    
    qDebug() << "[NetworkRangeWidget] Settings loaded, restored range:" << m_currentRange;
    
    // Refresh display
    refreshRanges();
    updateRangeList();
    
    // Emit signal that ranges are loaded
    emit rangesLoaded(count);
}

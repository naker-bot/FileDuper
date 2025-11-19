#include "settingsdialog.h"
#include <QTabWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent)
{
    // 🎯 Initialize default settings
    defaultSettings = {
        1000,           // threadCount
        true,           // useGpuAccel
        false,          // useNpuAccel
        true,           // enableEarlyExit
        true,           // enableMmap
        true,           // enableIouring
        200,            // chunkSize
        8 * 1024 * 1024,// blockSize
        true,           // enableReadahead
        true,           // skipHiddenFiles
        false,          // skipLargeFiles
        0,              // maxFileSizeMB
        "Alle Dateien", // fileFilter
        true,           // showDetailedProgress
        true,           // showHardwareStats
        false           // autoDeleteConfirm
    };

    currentSettings = defaultSettings;

    setWindowTitle("🎛️ FileDuper Settings - Optimierungen & Konfiguration");
    setMinimumWidth(600);
    setMinimumHeight(500);

    setupUI();
    connectSignals();
}

void SettingsDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 🎯 Tab Widget
    QTabWidget *tabWidget = new QTabWidget(this);

    createPerformanceTab();
    createMemoryTab();
    createScanningTab();
    createDisplayTab();

    tabWidget->addTab(new QWidget(), "⚡ Performance");   // Will be populated
    tabWidget->addTab(new QWidget(), "💾 Memory");       // Will be populated
    tabWidget->addTab(new QWidget(), "🔍 Scanning");     // Will be populated
    tabWidget->addTab(new QWidget(), "📊 Display");      // Will be populated

    // 🔧 Fix: Rebuild tabs with proper layouts
    QWidget *performanceTab = new QWidget();
    QVBoxLayout *perfLayout = new QVBoxLayout(performanceTab);

    // Performance Options Group
    QGroupBox *perfGroup = new QGroupBox("🚀 Performance Optimization", this);
    QVBoxLayout *perfGroupLayout = new QVBoxLayout(perfGroup);

    // Thread Count
    QHBoxLayout *threadLayout = new QHBoxLayout();
    threadLayout->addWidget(new QLabel("Max Parallel Threads:"));
    threadCountSpinBox = new QSpinBox();
    threadCountSpinBox->setMinimum(1);
    threadCountSpinBox->setMaximum(4096);
    threadCountSpinBox->setValue(currentSettings.threadCount);
    threadLayout->addWidget(threadCountSpinBox);
    threadLayout->addStretch();
    perfGroupLayout->addLayout(threadLayout);

    // GPU Acceleration
    useGpuCheckBox = new QCheckBox("🎮 Enable GPU Acceleration (CUDA/OpenCL)");
    useGpuCheckBox->setChecked(currentSettings.useGpuAccel);
    perfGroupLayout->addWidget(useGpuCheckBox);

    // NPU Acceleration
    useNpuCheckBox = new QCheckBox("🧠 Enable NPU Image Analysis");
    useNpuCheckBox->setChecked(currentSettings.useNpuAccel);
    perfGroupLayout->addWidget(useNpuCheckBox);

    // Early Exit Filter
    enableEarlyExitCheckBox = new QCheckBox("📊 Enable Early-Exit Filter (512B pre-hash)");
    enableEarlyExitCheckBox->setChecked(currentSettings.enableEarlyExit);
    perfGroupLayout->addWidget(enableEarlyExitCheckBox);

    // mmap for Large Files
    enableMmapCheckBox = new QCheckBox("💾 Enable mmap I/O for Files >10MB");
    enableMmapCheckBox->setChecked(currentSettings.enableMmap);
    perfGroupLayout->addWidget(enableMmapCheckBox);

    // io_uring
    enableIouringCheckBox = new QCheckBox("⚡ Enable io_uring Async I/O");
    enableIouringCheckBox->setChecked(currentSettings.enableIouring);
    perfGroupLayout->addWidget(enableIouringCheckBox);

    perfLayout->addWidget(perfGroup);
    perfLayout->addStretch();

    // Replace Tab 0 with performance tab
    tabWidget->removeTab(0);
    tabWidget->insertTab(0, performanceTab, "⚡ Performance");

    mainLayout->addWidget(tabWidget);

    // 🎯 Button Layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    applyBtn = new QPushButton("✅ Apply");
    resetBtn = new QPushButton("↩️ Reset");
    defaultsBtn = new QPushButton("🔄 Restore Defaults");
    closeBtn = new QPushButton("❌ Close");

    buttonLayout->addWidget(applyBtn);
    buttonLayout->addWidget(resetBtn);
    buttonLayout->addWidget(defaultsBtn);
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeBtn);

    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void SettingsDialog::createPerformanceTab()
{
    // 🚀 Performance settings already in setupUI()
}

void SettingsDialog::createMemoryTab()
{
    // 💾 Memory tab - similar structure
}

void SettingsDialog::createScanningTab()
{
    // 🔍 Scanning tab - similar structure
}

void SettingsDialog::createDisplayTab()
{
    // 📊 Display tab - similar structure
}

void SettingsDialog::connectSignals()
{
    connect(applyBtn, &QPushButton::clicked, this, &SettingsDialog::onApply);
    connect(resetBtn, &QPushButton::clicked, this, &SettingsDialog::onReset);
    connect(defaultsBtn, &QPushButton::clicked, this, &SettingsDialog::onRestoreDefaults);
    connect(closeBtn, &QPushButton::clicked, this, &QDialog::close);
}

void SettingsDialog::onApply()
{
    currentSettings.threadCount = threadCountSpinBox->value();
    currentSettings.useGpuAccel = useGpuCheckBox->isChecked();
    currentSettings.useNpuAccel = useNpuCheckBox->isChecked();
    currentSettings.enableEarlyExit = enableEarlyExitCheckBox->isChecked();
    currentSettings.enableMmap = enableMmapCheckBox->isChecked();
    currentSettings.enableIouring = enableIouringCheckBox->isChecked();

    QMessageBox::information(this, "✅ Settings Applied", 
        "Alle Einstellungen wurden übernommen!\n\nNächster Scan wird neue Optionen nutzen.");
    accept();
}

void SettingsDialog::onReset()
{
    threadCountSpinBox->setValue(currentSettings.threadCount);
    useGpuCheckBox->setChecked(currentSettings.useGpuAccel);
    useNpuCheckBox->setChecked(currentSettings.useNpuAccel);
    enableEarlyExitCheckBox->setChecked(currentSettings.enableEarlyExit);
    enableMmapCheckBox->setChecked(currentSettings.enableMmap);
    enableIouringCheckBox->setChecked(currentSettings.enableIouring);
}

void SettingsDialog::onRestoreDefaults()
{
    int ret = QMessageBox::question(this, "🔄 Restore Defaults",
        "Wirklich auf Standard-Einstellungen zurücksetzen?",
        QMessageBox::Yes | QMessageBox::Cancel);

    if (ret == QMessageBox::Yes) {
        currentSettings = defaultSettings;
        threadCountSpinBox->setValue(defaultSettings.threadCount);
        useGpuCheckBox->setChecked(defaultSettings.useGpuAccel);
        useNpuCheckBox->setChecked(defaultSettings.useNpuAccel);
        enableEarlyExitCheckBox->setChecked(defaultSettings.enableEarlyExit);
        enableMmapCheckBox->setChecked(defaultSettings.enableMmap);
        enableIouringCheckBox->setChecked(defaultSettings.enableIouring);
    }
}

SettingsDialog::Settings SettingsDialog::getSettings() const
{
    return currentSettings;
}

void SettingsDialog::setSettings(const Settings &settings)
{
    currentSettings = settings;
    threadCountSpinBox->setValue(settings.threadCount);
    useGpuCheckBox->setChecked(settings.useGpuAccel);
    useNpuCheckBox->setChecked(settings.useNpuAccel);
    enableEarlyExitCheckBox->setChecked(settings.enableEarlyExit);
    enableMmapCheckBox->setChecked(settings.enableMmap);
    enableIouringCheckBox->setChecked(settings.enableIouring);
}

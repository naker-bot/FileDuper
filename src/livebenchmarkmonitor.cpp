#include "livebenchmarkmonitor.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <iomanip>

LiveBenchmarkMonitor::LiveBenchmarkMonitor(QWidget *parent)
    : QDialog(parent)
{
    setupUI();
    
    // Update Timer für Live-Statistiken
    updateTimer = new QTimer(this);
    connect(updateTimer, &QTimer::timeout, this, &LiveBenchmarkMonitor::updateStatistics);
    updateTimer->setInterval(500); // Update alle 500ms
}

void LiveBenchmarkMonitor::setupUI()
{
    setWindowTitle("🎯 Live Benchmark Monitor - FileDuper Aktivitätsanzeige");
    setMinimumWidth(1000);
    setMinimumHeight(800);
    setStyleSheet("QDialog { background-color: #1e1e1e; color: white; }");

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 📊 Top Status Bar
    QHBoxLayout *statusLayout = new QHBoxLayout();
    
    statusLabel = new QLabel("🔄 Benchmark lädt...");
    statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #00FF00;");
    
    timeElapsedLabel = new QLabel("Zeit: 00:00:00");
    timeElapsedLabel->setStyleSheet("font-size: 12px; color: #00FF00;");
    
    speedLabel = new QLabel("Speed: 0 files/sec");
    speedLabel->setStyleSheet("font-size: 12px; color: #00FF00;");
    
    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(timeElapsedLabel);
    statusLayout->addWidget(speedLabel);
    
    mainLayout->addLayout(statusLayout);

    // 🖥️ Hardware Metrics Bar
    QGroupBox *hardwareGroup = new QGroupBox("⚡ Hardware Auslastung (Live)", this);
    hardwareGroup->setStyleSheet("QGroupBox { color: white; border: 2px solid #00AA00; }");
    QHBoxLayout *hardwareLayout = new QHBoxLayout(hardwareGroup);

    cpuUsageLabel = new QLabel("CPU: 0%");
    cpuUsageLabel->setStyleSheet("background-color: #333; padding: 5px; border-radius: 3px;");
    gpuUsageLabel = new QLabel("GPU: 0%");
    gpuUsageLabel->setStyleSheet("background-color: #333; padding: 5px; border-radius: 3px;");
    ioUsageLabel = new QLabel("I/O: 0%");
    ioUsageLabel->setStyleSheet("background-color: #333; padding: 5px; border-radius: 3px;");
    memoryUsageLabel = new QLabel("Memory: 0%");
    memoryUsageLabel->setStyleSheet("background-color: #333; padding: 5px; border-radius: 3px;");

    hardwareLayout->addWidget(cpuUsageLabel);
    hardwareLayout->addWidget(gpuUsageLabel);
    hardwareLayout->addWidget(ioUsageLabel);
    hardwareLayout->addWidget(memoryUsageLabel);
    hardwareLayout->addStretch();

    mainLayout->addWidget(hardwareGroup);

    // 📈 Progress Bar
    overallProgressBar = new QProgressBar();
    overallProgressBar->setMaximum(100);
    overallProgressBar->setStyleSheet(
        "QProgressBar { border: 2px solid #00AA00; border-radius: 5px; text-align: center; background-color: #000; }"
        "QProgressBar::chunk { background-color: #00FF00; }"
    );
    mainLayout->addWidget(overallProgressBar);

    // 📋 Statistics Table
    QGroupBox *statsGroup = new QGroupBox("📊 Scan-Statistiken", this);
    statsGroup->setStyleSheet("QGroupBox { color: white; border: 2px solid #00AA00; }");
    QVBoxLayout *statsLayout = new QVBoxLayout(statsGroup);

    statisticsTable = new QTableWidget(0, 2);
    statisticsTable->setColumnWidth(0, 300);
    statisticsTable->setColumnWidth(1, 200);
    statisticsTable->setHorizontalHeaderLabels({"Metrik", "Wert"});
    statisticsTable->setStyleSheet(
        "QTableWidget { background-color: #1a1a1a; color: white; border: 1px solid #333; }"
        "QHeaderView::section { background-color: #222; color: white; padding: 5px; border: 1px solid #444; }"
        "QTableWidget::item { padding: 5px; border: 1px solid #333; }"
    );
    statisticsTable->horizontalHeader()->setStretchLastSection(true);

    // Populate statistics
    statisticsTable->insertRow(0);
    statisticsTable->setItem(0, 0, new QTableWidgetItem("📂 Dateien gesammelt"));
    statisticsTable->setItem(0, 1, new QTableWidgetItem("0"));

    statisticsTable->insertRow(1);
    statisticsTable->setItem(1, 0, new QTableWidgetItem("🔍 Dateien gehasht"));
    statisticsTable->setItem(1, 1, new QTableWidgetItem("0"));

    statisticsTable->insertRow(2);
    statisticsTable->setItem(2, 0, new QTableWidgetItem("🎯 Duplikate gefunden"));
    statisticsTable->setItem(2, 1, new QTableWidgetItem("0"));

    statisticsTable->insertRow(3);
    statisticsTable->setItem(3, 0, new QTableWidgetItem("💾 Duplikat-Größe"));
    statisticsTable->setItem(3, 1, new QTableWidgetItem("0 GB"));

    statsLayout->addWidget(statisticsTable);
    mainLayout->addWidget(statsGroup);

    // 📝 Activity Log
    QGroupBox *logGroup = new QGroupBox("🔍 Live Activity Log", this);
    logGroup->setStyleSheet("QGroupBox { color: white; border: 2px solid #00AA00; }");
    QVBoxLayout *logLayout = new QVBoxLayout(logGroup);

    logTextEdit = new QTextEdit();
    logTextEdit->setReadOnly(true);
    logTextEdit->setStyleSheet(
        "QTextEdit { background-color: #0a0a0a; color: #00FF00; font-family: 'Courier New'; font-size: 10px; }"
    );
    logLayout->addWidget(logTextEdit);

    mainLayout->addWidget(logGroup);

    setLayout(mainLayout);
}

void LiveBenchmarkMonitor::addLogEntry(const QString &timestamp, const QString &category, 
                                       const QString &message, const QString &color)
{
    LogEntry entry{timestamp, category, message, color};
    activityLog.push_back(entry);

    // Limit log size
    if (activityLog.size() > MAX_LOG_ENTRIES) {
        activityLog.pop_front();
    }

    // Update display
    QString logLine = QString("<span style='color: %1;'>[%2] <b>%3</b>: %4</span>")
        .arg(color, timestamp, category, message);
    
    logTextEdit->append(logLine);

    // Auto-scroll to bottom
    QTextCursor cursor = logTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    logTextEdit->setTextCursor(cursor);
}

void LiveBenchmarkMonitor::onBenchmarkStarted()
{
    benchmarkStart = QDateTime::currentDateTime();
    statusLabel->setText("✅ Benchmark AKTIV - Scanning läuft...");
    statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #00FF00;");
    
    clearLog();
    addLogEntry(QDateTime::currentDateTime().toString("hh:mm:ss"), "BENCHMARK", 
                "🚀 Benchmark gestartet!", "#00FF00");
    
    updateTimer->start();
}

void LiveBenchmarkMonitor::onFilesCollected(int totalFiles)
{
    totalFilesCollected = totalFiles;
    addLogEntry(QDateTime::currentDateTime().toString("hh:mm:ss"), "COLLECTION",
                QString("✅ %1 Dateien gesammelt").arg(totalFiles), "#00FF00");
    updateStatistics();
}

void LiveBenchmarkMonitor::onHashingStarted(int filesToHash)
{
    addLogEntry(QDateTime::currentDateTime().toString("hh:mm:ss"), "HASHING",
                QString("🔐 Starte Hashing für %1 Dateien...").arg(filesToHash), "#FFAA00");
}

void LiveBenchmarkMonitor::onScanProgress(int percentage, int current, int total)
{
    overallProgressBar->setValue(percentage);
    filesHashedSoFar = current;
    
    if (current % 100 == 0) {
        double speed = (current > 0) ? (double)current / benchmarkStart.secsTo(QDateTime::currentDateTime()) : 0;
        speedLabel->setText(QString("Speed: %1 files/sec").arg(speed, 0, 'f', 1));
    }
}

void LiveBenchmarkMonitor::onCurrentFileProcessing(const QString &fileName, const QString &processType, 
                                                   int fileNumber, int totalFiles)
{
    if (fileNumber % 50 == 0) {  // Log nur jeden 50. für weniger Spam
        addLogEntry(QDateTime::currentDateTime().toString("hh:mm:ss"), processType,
                    QString("[%1/%2] %3").arg(fileNumber).arg(totalFiles).arg(fileName), "#CCCCCC");
    }
}

void LiveBenchmarkMonitor::onProcessActivityUpdate(const QString &activity, const QString &details)
{
    addLogEntry(QDateTime::currentDateTime().toString("hh:mm:ss"), "ACTIVITY",
                QString("%1 - %2").arg(activity, details), "#AAAAFF");
}

void LiveBenchmarkMonitor::onHardwareUsageUpdate(const QString &processingUnit, int utilization, 
                                                 const QString &task)
{
    if (processingUnit == "CPU") {
        lastCpuUsage = utilization;
        cpuUsageLabel->setText(QString("CPU: %1%").arg(utilization));
        if (utilization > 80) cpuUsageLabel->setStyleSheet("background-color: #FF3333; padding: 5px; border-radius: 3px;");
        else cpuUsageLabel->setStyleSheet("background-color: #333; padding: 5px; border-radius: 3px;");
    }
    else if (processingUnit == "GPU") {
        lastGpuUsage = utilization;
        gpuUsageLabel->setText(QString("GPU: %1%").arg(utilization));
        if (utilization > 80) gpuUsageLabel->setStyleSheet("background-color: #FF3333; padding: 5px; border-radius: 3px;");
        else gpuUsageLabel->setStyleSheet("background-color: #333; padding: 5px; border-radius: 3px;");
    }
    else if (processingUnit == "I/O") {
        lastIoUsage = utilization;
        ioUsageLabel->setText(QString("I/O: %1%").arg(utilization));
        if (utilization > 80) ioUsageLabel->setStyleSheet("background-color: #FF3333; padding: 5px; border-radius: 3px;");
        else ioUsageLabel->setStyleSheet("background-color: #333; padding: 5px; border-radius: 3px;");
    }

    addLogEntry(QDateTime::currentDateTime().toString("hh:mm:ss"), "HARDWARE",
                QString("%1 (%2%): %3").arg(processingUnit).arg(utilization).arg(task), "#FFFF00");
}

void LiveBenchmarkMonitor::onHashCalculated(const QString &filePath, const QString &hash)
{
    if (hash != "FTP_STREAM_FAILED_78" && !hash.startsWith("FTP_STREAM_FAILED")) {
        duplicatesFound++;
    }
}

void LiveBenchmarkMonitor::onBenchmarkFinished(double duration, int filesScanned, double speed)
{
    benchmarkEnd = QDateTime::currentDateTime();
    updateTimer->stop();

    statusLabel->setText(QString("✅ Benchmark ABGESCHLOSSEN - Duration: %1s").arg(duration, 0, 'f', 1));
    statusLabel->setStyleSheet("font-size: 14px; font-weight: bold; color: #00FFFF;");

    addLogEntry(QDateTime::currentDateTime().toString("hh:mm:ss"), "BENCHMARK_END",
                QString("🏁 Benchmark abgeschlossen! Duration: %1s | Speed: %2 files/sec").arg(duration, 0, 'f', 1).arg(speed, 0, 'f', 1),
                "#00FFFF");
}

void LiveBenchmarkMonitor::updateStatistics()
{
    // Update row 0: Files collected
    if (statisticsTable->rowCount() > 0) {
        statisticsTable->item(0, 1)->setText(QString::number(totalFilesCollected));
    }

    // Update row 1: Files hashed
    if (statisticsTable->rowCount() > 1) {
        statisticsTable->item(1, 1)->setText(QString::number(filesHashedSoFar));
    }

    // Update row 2: Duplicates
    if (statisticsTable->rowCount() > 2) {
        statisticsTable->item(2, 1)->setText(QString::number(duplicatesFound));
    }

    // Update row 3: Size
    if (statisticsTable->rowCount() > 3) {
        double sizeGB = duplicatesSizeGB;
        statisticsTable->item(3, 1)->setText(QString("%1 GB").arg(sizeGB, 0, 'f', 2));
    }

    // Update time elapsed
    if (benchmarkStart.isValid()) {
        int seconds = benchmarkStart.secsTo(QDateTime::currentDateTime());
        int hours = seconds / 3600;
        int minutes = (seconds % 3600) / 60;
        int secs = seconds % 60;
        timeElapsedLabel->setText(QString("Zeit: %1:%2:%3")
            .arg(hours, 2, 10, QChar('0'))
            .arg(minutes, 2, 10, QChar('0'))
            .arg(secs, 2, 10, QChar('0')));
    }
}

void LiveBenchmarkMonitor::clearLog()
{
    logTextEdit->clear();
    activityLog.clear();
    totalFilesCollected = 0;
    filesHashedSoFar = 0;
    duplicatesFound = 0;
    benchmarkStart = QDateTime();
    benchmarkEnd = QDateTime();
}

void LiveBenchmarkMonitor::exportLog(const QString &filename)
{
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "=== FileDuper Benchmark Activity Log ===\n\n";
        
        for (const auto &entry : activityLog) {
            out << QString("[%1] %2: %3\n").arg(entry.timestamp, entry.category, entry.message);
        }
        
        file.close();
    }
}

void LiveBenchmarkMonitor::onActivityLogged(const QString &activity, const QString &details)
{
    QString timestamp = QDateTime::currentDateTime().toString("hh:mm:ss");
    QString logEntry = QString("[%1] %2: %3").arg(timestamp, activity, details);
    
    // Log to activity log
    activityLog.push_back({timestamp, activity, details});
    if (activityLog.size() > 1000) {
        activityLog.pop_front();  // Keep max 1000 entries
    }
}

void LiveBenchmarkMonitor::onStatusChanged(const QString &status)
{
    onProcessActivityUpdate("Status", status);
}

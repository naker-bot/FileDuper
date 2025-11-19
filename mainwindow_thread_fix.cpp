// GUI Thread-Safety Fix für MainWindow - Ersetzt QApplication::processEvents()
// Diese Version implementiert korrekte Qt Threading Patterns

// 1. setupConnections() - Thread-sichere Signal/Slot Verbindungen
void MainWindow::setupConnections()
{
    qDebug() << "[MainWindow] 🔗 Thread-sichere Signal/Slot Verbindungen werden eingerichtet...";

    // ✅ KRITISCH: Verwende Qt::QueuedConnection für Cross-Thread Communication
    // Dies verhindert GUI-Blockierung und Race Conditions
    
    // Network Scanner Verbindungen
    connect(m_networkScanner, &NetworkScanner::serviceFound, this, [this](const NetworkService &service) {
        qDebug() << "[MainWindow] 🌐 Service gefunden:" << service.ip << ":" << service.port << "(" << service.serviceName << ")";
        updateNetworkServiceTree(service);
    }, Qt::QueuedConnection); // ← THREAD-SAFE
    
    // Scanner Progress Updates - QUEUED für Thread-Safety
    connect(m_scanner, &Scanner::scanProgress, this, &MainWindow::onScanProgress, Qt::QueuedConnection);
    connect(m_scanner, &Scanner::scanCompleted, this, [this](const DuplicateGroups &groups) {
        qDebug() << "[MainWindow] ✅ Scan abgeschlossen mit" << groups.size() << "Duplikat-Gruppen";
        onScanCompleted(groups);
        m_isScanning = false;
        enableScanControls(true);
    }, Qt::QueuedConnection); // ← THREAD-SAFE
    
    // Hardware Usage Updates - ebenfalls QUEUED
    connect(m_scanner, &Scanner::hardwareUsageUpdate, this, &MainWindow::onHardwareUsageUpdate, Qt::QueuedConnection);
    
    // FTP Client Verbindungen - THREAD-SAFE
    connect(m_ftpClient, &FtpClient::listFinished, this, [this](const QStringList &dirs, bool success) {
        if (success) {
            qDebug() << "[MainWindow] 📡 FTP Verzeichnisse empfangen:" << dirs.size();
            updateFtpDirectoryTree(dirs);
        }
    }, Qt::QueuedConnection);
    
    connect(m_ftpClient, &FtpClient::error, this, [this](const QString &error) {
        QMessageBox::warning(this, "FTP Fehler", error);
    }, Qt::QueuedConnection);
    
    // Activity Indicator Updates
    connect(m_activityIndicator, &ActivityIndicator::hardwareLoadChanged, this, 
            &MainWindow::onHardwareLoadChanged, Qt::QueuedConnection);
}

// 2. onScanProgress() - GUI Updates ohne processEvents()
void MainWindow::onScanProgress(int percentage, int current, int total)
{
    // 🚫 ENTFERNT: QApplication::processEvents() - verursacht Deadlocks!
    // ✅ NEU: Direkte GUI Updates - Qt's Event Loop verarbeitet diese automatisch
    
    qDebug() << "[MainWindow] 📊 onScanProgress aufgerufen:" << percentage << "% (" << current << "/" << total << ")";
    
    if (total > 0) {
        // Präzise Prozent-Berechnung
        double precisePercentage = (double(current) / double(total)) * 100.0;
        int displayPercentage = qMax(1, qRound(precisePercentage));
        
        // GUI Updates - THREAD-SAFE da wir im Main Thread sind
        progressBar->setMaximum(100);
        progressBar->setValue(displayPercentage);
        
        // Status-abhängige Aktions-Anzeige
        QString currentAction;
        if (displayPercentage >= 90) {
            currentAction = "🔍 Identifiziere Duplikate...";
            if (fileComparisonLabel) fileComparisonLabel->setText("🔍 Duplikat-Vergleich aktiv");
        } else if (displayPercentage >= 50) {
            currentAction = "🔐 Hash-Berechnung läuft...";
            if (fileComparisonLabel) fileComparisonLabel->setText("🔐 Hash-Berechnung");
        } else if (displayPercentage >= 10) {
            currentAction = "📏 Analysiere Dateigrößen...";
            if (fileComparisonLabel) fileComparisonLabel->setText("📏 Größen-Sortierung");
        } else {
            currentAction = "🔍 Sammle Dateien...";
            if (fileComparisonLabel) fileComparisonLabel->setText("📂 Datei-Sammlung");
        }
        
        // GUI Component Updates
        actionLabel->setText(currentAction);
        fileCountLabel->setText(QString("Datei %1 von %2 (%3%)").arg(current).arg(total).arg(QString::number(precisePercentage, 'f', 2)));
        statusBar()->showMessage(QString("🔍 Hash-Berechnung: %1/%2 (%3%)").arg(current).arg(total).arg(percentage));
        
        // ✅ THREAD-SAFE: Qt's Event Loop verarbeitet Updates automatisch
        // Kein processEvents() nötig - das blockiert nur!
        
        qDebug() << "[MainWindow] ✅ GUI-Update completed thread-safe";
    } else {
        // Unbestimmter Progress
        progressBar->setMaximum(0);
        actionLabel->setText("🔍 Initialisiere Scan...");
    }
}

// 3. Responsive GUI Timer - verhindert Einfrieren
void MainWindow::initializeResponsiveGUI()
{
    // ✅ GUI Responsiveness Timer - ersetzt processEvents()
    m_guiUpdateTimer = new QTimer(this);
    m_guiUpdateTimer->setInterval(100); // 100ms Updates
    connect(m_guiUpdateTimer, &QTimer::timeout, this, [this]() {
        // Sanfte GUI Updates ohne Blocking
        if (m_isScanning) {
            // Activity Indicator Updates
            if (m_activityIndicator) {
                m_activityIndicator->updateDisplayValues();
            }
            
            // Hardware Load Indicator blinken lassen bei hoher Auslastung
            updateHardwareLoadIndicator();
        }
    });
    
    // Timer starten
    m_guiUpdateTimer->start();
    qDebug() << "[MainWindow] ✅ Responsive GUI Timer gestartet (100ms)";
}

// 4. Hardware Load Updates - Thread-Safe
void MainWindow::updateHardwareLoadIndicator()
{
    // Diese Funktion läuft im GUI Timer - kein Threading Problem
    if (!m_activityIndicator) return;
    
    // Get current hardware loads
    int cpuLoad = m_activityIndicator->getCurrentCpuLoad();
    int gpuLoad = m_activityIndicator->getCurrentGpuLoad();
    int npuLoad = m_activityIndicator->getCurrentNpuLoad();
    
    // Update display without blocking
    if (cpuLoadLabel) {
        cpuLoadLabel->setText(QString("CPU: %1%").arg(cpuLoad));
        cpuLoadLabel->setStyleSheet(getLoadColorStyle(cpuLoad));
    }
    
    if (gpuLoadLabel) {
        gpuLoadLabel->setText(QString("GPU: %1%").arg(gpuLoad));
        gpuLoadLabel->setStyleSheet(getLoadColorStyle(gpuLoad));
    }
    
    if (npuLoadLabel) {
        npuLoadLabel->setText(QString("NPU: %1%").arg(npuLoad));
        npuLoadLabel->setStyleSheet(getLoadColorStyle(npuLoad));
    }
    
    // Blink animation für hohe Auslastung
    bool shouldBlink = cpuLoad > 80 || gpuLoad > 80 || npuLoad > 80;
    if (shouldBlink != m_hardwareIndicatorVisible) {
        m_hardwareIndicatorVisible = !m_hardwareIndicatorVisible;
        
        // Blink effect für kritische Werte
        if (cpuLoad > 90) {
            cpuLoadLabel->setVisible(m_hardwareIndicatorVisible);
        }
        if (gpuLoad > 90) {
            gpuLoadLabel->setVisible(m_hardwareIndicatorVisible);
        }
        if (npuLoad > 90) {
            npuLoadLabel->setVisible(m_hardwareIndicatorVisible);
        }
    }
}

// 5. Load Color Styling
QString MainWindow::getLoadColorStyle(int load)
{
    if (load >= 90) {
        return "QLabel { color: #FF0000; font-weight: bold; }"; // Red
    } else if (load >= 70) {
        return "QLabel { color: #FFA500; font-weight: bold; }"; // Orange
    } else if (load >= 30) {
        return "QLabel { color: #FFFF00; font-weight: bold; }"; // Yellow
    } else {
        return "QLabel { color: #00FF00; font-weight: bold; }"; // Green
    }
}

// 6. Scanner Threading Setup
void MainWindow::setupScannerThreading()
{
    // ✅ Scanner in separaten Thread verschieben
    m_scannerThread = new QThread(this);
    m_scanner->moveToThread(m_scannerThread);
    
    // Thread Lifecycle Management
    connect(m_scannerThread, &QThread::started, m_scanner, &Scanner::initialize);
    connect(m_scanner, &Scanner::finished, m_scannerThread, &QThread::quit);
    connect(m_scannerThread, &QThread::finished, m_scannerThread, &QThread::deleteLater);
    
    // Start Scanner Thread
    m_scannerThread->start();
    
    qDebug() << "[MainWindow] ✅ Scanner Thread gestartet";
}

// 7. Cleanup bei Anwendungsende
void MainWindow::closeEvent(QCloseEvent *event)
{
    // Stoppe alle Timer
    if (m_guiUpdateTimer) {
        m_guiUpdateTimer->stop();
    }
    
    // Stoppe Scanner Thread sauber
    if (m_scannerThread && m_scannerThread->isRunning()) {
        m_scanner->stopScan();
        m_scannerThread->quit();
        m_scannerThread->wait(3000); // 3 Sekunden warten
    }
    
    // Speichere Einstellungen
    saveSettings();
    
    event->accept();
    qDebug() << "[MainWindow] ✅ Anwendung thread-safe beendet";
}

// Diese Implementierung löst die GUI-Responsiveness Probleme durch:
// 1. Qt::QueuedConnection für alle Cross-Thread Signals
// 2. Entfernung von QApplication::processEvents()
// 3. Timer-basierte GUI Updates (100ms)
// 4. Korrekte Thread-Trennung zwischen Scanner und GUI
// 5. Thread-sichere Hardware Load Monitoring
#include "mainwindow.h"
#include <QApplication>
#include <QMenuBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QDebug>
#include <QDir>
#include <QFileInfo>

// Simple Impl structure for PImpl pattern
struct MainWindow::Impl
{
    bool isScanning = false;
    QStringList scanDirectories;
};

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_selectedDirectories()
    , directorySummaryLabel(nullptr)
    , progressBar(nullptr)
    , resultsTable(nullptr)
    , m_scanner(nullptr)
    , m_networkScanner(nullptr)
    , m_hashEngine(nullptr)
    , m_npuManager(nullptr)
    , m_presetManager(nullptr)
    , m_activityIndicator(nullptr)
    , d(std::make_unique<Impl>())
{
    qDebug() << "[MainWindow] 🚀 CONSTRUCTOR gestartet - Minimal Clean Version";
    
    setWindowTitle("FileDuper - Advanced Duplicate Scanner");
    setMinimumSize(800, 600);
    
    setupMinimalGUI();
    
    qDebug() << "[MainWindow] ✅ CONSTRUCTOR erfolgreich beendet";
}

MainWindow::~MainWindow()
{
    qDebug() << "[MainWindow] 🛑 DESTRUCTOR";
}

void MainWindow::setupMinimalGUI()
{
    qDebug() << "[MainWindow] 🎨 setupMinimalGUI() gestartet - CRASH-SAFE VERSION";
    
    setWindowTitle(tr("FileDuper - Stabile Version"));
    resize(800, 600);
    
    // Zentrales Widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Directory Summary Label
    if (!directorySummaryLabel) {
        directorySummaryLabel = new QLabel(tr("📁 Keine Verzeichnisse ausgewählt\n\nKlicken Sie auf 'Verzeichnis hinzufügen' um zu beginnen."), this);
        directorySummaryLabel->setStyleSheet(
            "QLabel { "
            "   background-color: #f0f0f0; "
            "   border: 1px solid #ccc; "
            "   border-radius: 5px; "
            "   padding: 10px; "
            "   font-size: 12px; "
            "   min-height: 100px; "
            "}"
        );
        directorySummaryLabel->setWordWrap(true);
        directorySummaryLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    }
    mainLayout->addWidget(directorySummaryLabel);
    
    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    
    QPushButton *addDirBtn = new QPushButton(tr("📂 Verzeichnis hinzufügen"));
    connect(addDirBtn, &QPushButton::clicked, this, &MainWindow::addDirectory);
    
    QPushButton *clearBtn = new QPushButton(tr("🗑️ Alle entfernen"));
    connect(clearBtn, &QPushButton::clicked, [this]() {
        m_selectedDirectories.clear();
        updateDirectorySummary();
    });
    
    buttonLayout->addWidget(addDirBtn);
    buttonLayout->addWidget(clearBtn);
    buttonLayout->addStretch();
    
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch();
    
    statusBar()->showMessage(tr("FileDuper bereit - Minimale GUI-Version"));
    
    qDebug() << "[MainWindow] ✅ setupMinimalGUI() erfolgreich abgeschlossen";
}

void MainWindow::setupProgrammaticGUI()
{
    qDebug() << "[MainWindow] ⚠️ setupProgrammaticGUI() deaktiviert - verwende setupMinimalGUI()";
    // Original-Funktion deaktiviert, da sie Crashes verursacht
    // Alle alten GUI-Komponenten temporär entfernt für Stabilität
}

void MainWindow::addDirectory()
{
    qDebug() << "[MainWindow] 🔍 addDirectory() gestartet";
    
    if (!this) {
        qDebug() << "[MainWindow] ⚠️ MainWindow ist null - addDirectory abgebrochen";
        return;
    }
    
    qDebug() << "[MainWindow] 📂 QFileDialog wird geöffnet...";
    
    QString dir;
    try {
        dir = QFileDialog::getExistingDirectory(
            this, 
            tr("Verzeichnis zum Scannen auswählen"),
            QStandardPaths::writableLocation(QStandardPaths::HomeLocation),
            QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks
        );
        
        qDebug() << "[MainWindow] 📂 Dialog Ergebnis:" << dir;
        
    } catch (const std::exception &e) {
        qDebug() << "[MainWindow] ❌ Fehler beim QFileDialog:" << e.what();
        
        QString homeDir = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
        dir = homeDir + "/Documents";
        if (!QDir(dir).exists()) {
            dir = homeDir;
        }
        qDebug() << "[MainWindow] 🔄 Fallback-Verzeichnis:" << dir;
        
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unbekannter Fehler beim QFileDialog";
        return;
    }
    
    qDebug() << "[MainWindow] 📂 Verwende Verzeichnis:" << dir;
    
    if (!dir.isEmpty()) {
        if (!m_selectedDirectories.contains(dir)) {
            qDebug() << "[MainWindow] 📝 Füge Verzeichnis zur Liste hinzu...";
            m_selectedDirectories.append(dir);
            qDebug() << "[MainWindow] 📁 Verzeichnis hinzugefügt:" << dir;
            
            qDebug() << "[MainWindow] 🔄 Rufe updateDirectorySummary auf...";
            try {
                updateDirectorySummary();
                qDebug() << "[MainWindow] ✅ GUI-Update nach addDirectory erfolgreich";
            } catch (const std::exception &e) {
                qDebug() << "[MainWindow] ❌ Fehler bei updateDirectorySummary:" << e.what();
            } catch (...) {
                qDebug() << "[MainWindow] ❌ Unbekannter Fehler bei updateDirectorySummary";
            }
            
            qDebug() << "[MainWindow] 📊 Aktualisiere Status...";
            if (statusBar()) {
                statusBar()->showMessage(tr("📁 %1 Verzeichnisse ausgewählt - %2").arg(m_selectedDirectories.size()).arg(QFileInfo(dir).fileName()), 5000);
            }
        } else {
            qDebug() << "[MainWindow] ⚠️ Verzeichnis bereits vorhanden:" << dir;
            if (statusBar()) {
                statusBar()->showMessage(tr("⚠️ Verzeichnis bereits ausgewählt: %1").arg(QFileInfo(dir).fileName()), 3000);
            }
        }
    } else {
        qDebug() << "[MainWindow] ❌ Kein Verzeichnis ausgewählt";
    }
    
    qDebug() << "[MainWindow] ✅ addDirectory() beendet";
}

void MainWindow::updateDirectorySummary()
{
    qDebug() << "[MainWindow] 🔄 updateDirectorySummary() gestartet";
    
    if (!directorySummaryLabel) {
        qDebug() << "[MainWindow] ⚠️ directorySummaryLabel ist null - erstelle Fallback";
        directorySummaryLabel = new QLabel(tr("📁 Fallback Label erstellt"), this);
        return;
    }
    
    QString summaryText;
    
    if (m_selectedDirectories.isEmpty()) {
        summaryText = tr("📁 Keine Verzeichnisse ausgewählt\n\nKlicken Sie auf 'Verzeichnis hinzufügen' um zu beginnen.");
    } else {
        summaryText = tr("📁 Ausgewählte Verzeichnisse (%1):\n\n").arg(m_selectedDirectories.size());
        
        for (int i = 0; i < m_selectedDirectories.size(); ++i) {
            const QString &dir = m_selectedDirectories.at(i);
            QFileInfo info(dir);
            summaryText += tr("📂 %1\n").arg(info.absoluteFilePath());
            
            // Begrenze die Anzeige auf maximal 10 Verzeichnisse
            if (i >= 9 && m_selectedDirectories.size() > 10) {
                summaryText += tr("... und %1 weitere\n").arg(m_selectedDirectories.size() - 10);
                break;
            }
        }
    }
    
    try {
        directorySummaryLabel->setText(summaryText);
        qDebug() << "[MainWindow] ✅ Label erfolgreich aktualisiert mit" << m_selectedDirectories.size() << "Verzeichnissen";
    } catch (const std::exception &e) {
        qDebug() << "[MainWindow] ❌ Fehler beim Label-Update:" << e.what();
    } catch (...) {
        qDebug() << "[MainWindow] ❌ Unbekannter Fehler beim Label-Update";
    }
    
    qDebug() << "[MainWindow] ✅ updateDirectorySummary() beendet";
}

// Dummy-Implementierungen für andere Funktionen
void MainWindow::startDuplicateScan() { qDebug() << "[MainWindow] startDuplicateScan() - Dummy"; }
void MainWindow::onScanProgress(int, int, int) { qDebug() << "[MainWindow] onScanProgress() - Dummy"; }
void MainWindow::onScanCompleted(const QList<QStringList>&) { qDebug() << "[MainWindow] onScanCompleted() - Dummy"; }
void MainWindow::showResultsContextMenu(const QPoint&) { qDebug() << "[MainWindow] showResultsContextMenu() - Dummy"; }

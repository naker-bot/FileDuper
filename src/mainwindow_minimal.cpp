#include "mainwindow.h"
#include "presetmanager.h"
#include "activityindicator.h"
#include "ftpclient.h"
#include "logindialog.h"
#include "hashengine.h"
#include "networkrangewidget.h"

#include <QApplication>
#include <QMenuBar>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QProgressBar>
#include <QDebug>

// Minimal MainWindow implementation
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("FileDuper - Pool Engine Optimized");
    setGeometry(100, 100, 1200, 800);
    
    // Create central widget with basic layout
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    // Add basic controls
    QPushButton *startScanBtn = new QPushButton("🔍 Start Pool Engine Scan", this);
    QPushButton *stopScanBtn = new QPushButton("⏹️ Stop Scan", this);
    
    layout->addWidget(startScanBtn);
    layout->addWidget(stopScanBtn);
    
    // Add progress bar
    QProgressBar *progressBar = new QProgressBar(this);
    layout->addWidget(progressBar);
    
    // Add status label
    QLabel *statusLabel = new QLabel("Ready - Pool Engine: ACTIVE", this);
    layout->addWidget(statusLabel);
    
    // Create scanner and hash engine
    m_scanner = new Scanner(this);
    m_hashEngine = new HashEngine(this);
    m_presetManager = new PresetManager(this);
    
    qDebug() << "✅ MainWindow initialized with pool engine support";
}

MainWindow::~MainWindow()
{
    qDebug() << "🔄 MainWindow destructor";
}

// Minimal slot implementations  
void MainWindow::onScanProgress(int percentage, int current, int total)
{
    qDebug() << "📊 Scan Progress:" << percentage << "%" << current << "/" << total;
}

void MainWindow::onScanCompleted(const DuplicateGroups &results)
{
    qDebug() << "✅ Scan completed with" << results.groups.size() << "duplicate groups";
}

void MainWindow::onHardwareUsageUpdate(const QString &processingUnit, int utilizationPercent, const QString &currentTask)
{
    qDebug() << "🔧 Hardware:" << processingUnit << utilizationPercent << "%" << currentTask;
}

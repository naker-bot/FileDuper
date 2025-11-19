#include <QCoreApplication>
#include <QDebug>
#include "lightningscan.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== Lightning Scan Initialization Test ===";
    
    // Test 1: Create LightningScan object
    LightningScan *scanner = new LightningScan();
    qDebug() << "✅ LightningScan object created successfully";
    
    // Test 2: Set speed mode
    scanner->setSpeedMode(LightningScan::TURBO_MODE);
    qDebug() << "✅ TURBO_MODE set successfully";
    
    // Test 3: Check if scanning works
    QStringList testDirs;
    testDirs << "/tmp";  // Simple test directory
    
    qDebug() << "🚀 Starting test scan on /tmp...";
    scanner->startScan(testDirs);
    
    // Wait a bit for scan to start
    QTimer::singleShot(2000, &app, &QCoreApplication::quit);
    
    return app.exec();
}

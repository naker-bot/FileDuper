// Emergency FTP Crash Prevention Fix
// Add this to mainwindow.cpp to prevent crashes

// BEFORE calling any FTP function:
// 1. Check if FtpClient is valid
// 2. Add try-catch blocks
// 3. Validate all pointers

void SafeFtpOperationWrapper() {
    // Defensive pattern
    try {
        if (!m_ftpClient) {
            qCritical() << "❌ m_ftpClient is NULL!";
            QMessageBox::warning(this, "FTP Error", 
                "FTP Client not initialized. Use Local Scan instead.");
            return;
        }
        
        if (!m_scanner) {
            qCritical() << "❌ m_scanner is NULL!";
            return;
        }
        
        // Only proceed if both are valid
        // FTP operation here...
        
    } catch (const std::exception &e) {
        qCritical() << "Exception in FTP operation:" << e.what();
    } catch (...) {
        qCritical() << "Unknown exception in FTP operation";
    }
}

// Alternative: Disable FTP entirely
void DisableFtpFeatures() {
    // Comment out or hide FTP-related UI buttons
    // m_ftpScanButton->setVisible(false);  // Hide button
    // Or completely disable FtpDirectoryDialog instantiation
}

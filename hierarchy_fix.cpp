// Verbesserte Hierarchie-Konstruktion für FTP-Verzeichnisse
// Komplett neue Implementation mit rekursiver Parent-Erstellung

void MainWindow::buildFtpHierarchy(QTreeWidget *tree, const QStringList &directories, QLabel *statusLabel, QPushButton *selectBtn) {
    // Clear old content
    tree->clear();
    
    // Create root item
    QTreeWidgetItem *rootItem = new QTreeWidgetItem(tree);
    rootItem->setText(0, "📁 FTP Server Root");
    rootItem->setText(1, "🌐");
    rootItem->setText(2, QString("%1 Verzeichnisse").arg(directories.size()));
    rootItem->setData(0, Qt::UserRole, "/");
    rootItem->setExpanded(true);
    
    // Path to TreeItem mapping
    QMap<QString, QTreeWidgetItem*> pathMap;
    pathMap["/"] = rootItem;
    
    // Recursive function to ensure parent exists
    std::function<QTreeWidgetItem*(const QString&)> ensureParentExists = [&](const QString &fullPath) -> QTreeWidgetItem* {
        // Normalize path
        QString cleanPath = fullPath;
        if (!cleanPath.startsWith("/")) cleanPath = "/" + cleanPath;
        if (cleanPath.endsWith("/") && cleanPath != "/") cleanPath.chop(1);
        
        // Check if already exists
        if (pathMap.contains(cleanPath)) {
            return pathMap[cleanPath];
        }
        
        // Root case
        if (cleanPath == "/" || cleanPath.isEmpty()) {
            return rootItem;
        }
        
        // Find parent path
        QString parentPath = cleanPath.section('/', 0, -2);
        if (parentPath.isEmpty()) parentPath = "/";
        
        // Ensure parent exists recursively
        QTreeWidgetItem *parentItem = ensureParentExists(parentPath);
        
        // Create new item for current path
        QString itemName = cleanPath.section('/', -1);
        if (itemName.isEmpty()) return rootItem;
        
        QTreeWidgetItem *newItem = new QTreeWidgetItem(parentItem);
        newItem->setText(0, QString("📁 %1").arg(itemName));
        newItem->setText(1, "📂");
        newItem->setText(2, "Verzeichnis");
        newItem->setData(0, Qt::UserRole, cleanPath);
        newItem->setExpanded(false);
        
        pathMap[cleanPath] = newItem;
        return newItem;
    };
    
    // Process all directories with parent creation
    QStringList sortedDirs = directories;
    sortedDirs.sort();
    
    int processedCount = 0;
    for (const QString &dirPath : sortedDirs) {
        if (processedCount >= 300) break; // Higher limit
        
        // Skip root and empty paths
        if (dirPath == "/" || dirPath.isEmpty()) continue;
        
        // Ensure directory and all parents exist
        QTreeWidgetItem *dirItem = ensureParentExists(dirPath);
        
        // Mark as FTP-scanned directory
        if (dirItem) {
            dirItem->setText(2, "FTP-Verzeichnis");
            dirItem->setToolTip(0, QString("FTP-Pfad: %1").arg(dirPath));
        }
        
        processedCount++;
    }
    
    // Auto-expand first level
    for (int i = 0; i < rootItem->childCount(); ++i) {
        QTreeWidgetItem *child = rootItem->child(i);
        child->setExpanded(true);
        
        // Show structure but don't auto-expand all
        for (int j = 0; j < child->childCount() && j < 2; ++j) {
            child->child(j)->setExpanded(false);
        }
    }
    
    statusLabel->setText(QString("✅ %1 Verzeichnisse in Hierarchie geladen - Vollständige Eltern-Kind-Struktur").arg(processedCount));
    statusLabel->setStyleSheet("color: green; font-weight: bold;");
    selectBtn->setEnabled(true);
    
    // Double-click expansion
    connect(tree, &QTreeWidget::itemDoubleClicked, [=](QTreeWidgetItem *item) {
        QString itemPath = item->data(0, Qt::UserRole).toString();
        qDebug() << "[FTP Dialog] 🖱️ Verzeichnis expandiert/kollabiert:" << itemPath;
        item->setExpanded(!item->isExpanded());
    });
}
void MainWindow::loadFtpDirectoryTree(QTreeWidget *tree, const QString &ip, int port,
                                      const QString &username, const QString &password, QLabel *statusLabel)
{
    // ✅ ECHTER FTP Directory Tree mit hierarchischer Parent-Child-Struktur
    statusLabel->setText(tr("📡 FTP: Verbinde mit %1...").arg(ip));

    // FtpClient verwenden für echte FTP-Verbindung mit libcurl
    FtpClient *ftpClient = new FtpClient(this);
    ftpClient->setCredentials(ip, port, username, password);

    // Verbindung und Directory-Listing
    connect(ftpClient, &FtpClient::connected, [=]() {
        statusLabel->setText(tr("🔗 FTP: Verbunden mit %1").arg(ip));
        
        // Starte hierarchisches Directory Listing für Root (rekursiv)
        ftpClient->listDirectory("/");
    });

    connect(ftpClient, &FtpClient::directoryListingReceived, [=](const QStringList &directories) {
        if (!directories.isEmpty()) {
            tree->clear();
            
            std::cout << "🌐 FTP: Building hierarchical tree from " << directories.size() 
                      << " directories from " << ip.toStdString() << std::endl;
            
            // Root-Element
            QTreeWidgetItem *rootItem = new QTreeWidgetItem(tree);
            rootItem->setText(0, "/");
            rootItem->setText(1, "📁 FTP Root");
            rootItem->setText(2, QString::number(directories.size()) + " dirs");
            rootItem->setData(0, Qt::UserRole, "/");
            rootItem->setData(0, Qt::UserRole + 1, port);
            rootItem->setIcon(0, QIcon(":/icons/folder.png"));

            // Hierarchische Verzeichnisstruktur erstellen (Parent-Child-Beziehungen)
            QMap<QString, QTreeWidgetItem*> pathToItem;
            pathToItem["/"] = rootItem;

            // Sortiere Verzeichnisse nach Pfadtiefe (Parent-vor-Child)
            QStringList sortedDirs = directories;
            std::sort(sortedDirs.begin(), sortedDirs.end(), [](const QString &a, const QString &b) {
                int depthA = a.count('/');
                int depthB = b.count('/');
                if (depthA != depthB) return depthA < depthB;
                return a < b;  // Alphabetisch bei gleicher Tiefe
            });

            for (const QString &fullPath : sortedDirs) {
                if (fullPath.isEmpty() || fullPath == "/") continue;
                
                QString dirPath = fullPath;
                if (dirPath.endsWith('/')) dirPath.chop(1);  // Remove trailing slash
                
                QString parentPath = "/";
                QStringList pathParts = dirPath.split('/', Qt::SkipEmptyParts);
                QString dirName = pathParts.isEmpty() ? dirPath : pathParts.last();
                
                // Bestimme Parent-Pfad
                if (pathParts.size() > 1) {
                    pathParts.removeLast();  // Remove current directory name
                    parentPath = "/" + pathParts.join("/") + "/";
                }
                
                // Finde oder erstelle Parent-Item
                QTreeWidgetItem *parentItem = pathToItem.value(parentPath, rootItem);
                
                // Erstelle neues Verzeichnis-Item als Child des Parent
                QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
                item->setText(0, dirName);
                item->setText(1, "📂 FTP Child");
                item->setText(2, "---");
                item->setData(0, Qt::UserRole, fullPath);  // Store full path
                item->setData(0, Qt::UserRole + 1, port);
                item->setIcon(0, QIcon(":/icons/folder.png"));
                
                // Registriere Item für weitere Child-Verzeichnisse
                QString normalizedPath = fullPath;
                if (!normalizedPath.endsWith('/')) normalizedPath += "/";
                pathToItem[normalizedPath] = item;
                
                std::cout << "📁 FTP Child created: " << dirName.toStdString() 
                         << " (Parent: " << parentPath.toStdString() 
                         << ", Full: " << fullPath.toStdString() << ")" << std::endl;
            }

            // Expandiere erste 2 Ebenen für bessere Parent-Child-Sichtbarkeit
            rootItem->setExpanded(true);
            for (int i = 0; i < rootItem->childCount(); ++i) {
                QTreeWidgetItem *child = rootItem->child(i);
                child->setExpanded(true);
                
                // Expandiere auch die erste Ebene der Kinder
                for (int j = 0; j < child->childCount() && j < 3; ++j) {
                    child->child(j)->setExpanded(true);
                }
            }

            statusLabel->setText(tr("✅ FTP: %1 hierarchische Verzeichnisse mit Parent-Child-Struktur geladen").arg(directories.size()));
            
            std::cout << "🌳 FTP Hierarchical Tree complete: " << directories.size() 
                      << " directories in Parent-Child structure from " << ip.toStdString() << std::endl;
        } else {
            statusLabel->setText(tr("❌ FTP: Fehler beim Directory-Listing"));
        }
        
        ftpClient->deleteLater(); // Cleanup
    });

    connect(ftpClient, &FtpClient::error, [=](const QString &errorMsg) {
        statusLabel->setText(tr("❌ FTP: %1").arg(errorMsg));
        ftpClient->deleteLater();
    });

    // Starte Verbindung
    ftpClient->connectToHost();
}

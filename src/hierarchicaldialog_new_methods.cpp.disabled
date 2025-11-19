// Neue Methoden für HierarchicalDialog - Hinzufügen zu hierarchicaldialog.cpp

// Neue Methode: Lade eine spezifische Verzeichnisebene
void HierarchicalDialog::loadDirectoryLevel(const QString &dirPath, int depth)
{
    if (depth > 20) return; // Maximale Tiefe erreicht
    
    QString cleanDirPath = dirPath;
    if (!cleanDirPath.endsWith('/')) cleanDirPath += "/";
    
    // FTP LIST für aktuelles Verzeichnis
    CURL *curl = curl_easy_init();
    if (!curl) return;
    
    QString url = QString("ftp://%1:%2%3").arg(m_host).arg(m_port).arg(cleanDirPath);
    QString listing;
    
    curl_easy_setopt(curl, CURLOPT_URL, url.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_USERNAME, m_user.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, m_pass.toUtf8().constData());
    curl_easy_setopt(curl, CURLOPT_FTP_USE_EPSV, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeToString);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &listing);
    
    CURLcode res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    
    if (res == CURLE_OK) {
        QStringList subdirs;
        QTextStream stream(&listing);
        while (!stream.atEnd()) {
            QString line = stream.readLine().trimmed();
            if (line.startsWith('d')) {
                // ProFTPD Format parsen: drwxr-xr-x ... dirname
                QStringList parts = line.split(QRegularExpression("\\s+"));
                if (parts.size() >= 9) {
                    QString dirName = parts.last();
                    if (dirName != "." && dirName != "..") {
                        subdirs << dirName;
                    }
                }
            }
        }
        
        // In GUI-Thread einfügen
        if (!subdirs.isEmpty()) {
            QMetaObject::invokeMethod(this, [this, cleanDirPath, subdirs, depth]() {
                QTreeWidgetItem *parentItem = m_pathToItem.value(cleanDirPath, nullptr);
                if (parentItem) {
                    // Status aktualisieren
                    if (cleanDirPath == "/") {
                        parentItem->setText(0, QString("📡 %1 (%2 Hauptverzeichnisse)").arg(m_host).arg(subdirs.size()));
                    }
                    
                    for (const QString &subdir : subdirs) {
                        QString fullPath = cleanDirPath;
                        if (!fullPath.endsWith('/')) fullPath += "/";
                        fullPath += subdir;
                        if (!fullPath.endsWith('/')) fullPath += "/";
                        
                        // Neues aufklappbares Item erstellen
                        QTreeWidgetItem *item = new QTreeWidgetItem(parentItem);
                        item->setText(0, QString("📁 %1").arg(subdir));
                        item->setCheckState(0, Qt::Unchecked);
                        item->setData(0, Qt::UserRole, fullPath);
                        item->setData(0, Qt::UserRole + 1, depth + 1); // Tiefe speichern
                        item->setData(0, Qt::UserRole + 2, false); // Kinder geladen?
                        item->setExpanded(false);
                        
                        // Dummy Child hinzufügen um Expand-Button zu zeigen
                        QTreeWidgetItem *dummy = new QTreeWidgetItem(item);
                        dummy->setText(0, "⏳ Wird beim Aufklappen geladen...");
                        dummy->setData(0, Qt::UserRole + 3, true); // Marker: Dummy Item
                        dummy->setDisabled(true);
                        
                        m_pathToItem[fullPath] = item;
                        
                        // Tooltip für Pfad und Tiefe
                        item->setToolTip(0, QString("Pfad: %1\nTiefe: %2\nZum Aufklappen anklicken").arg(fullPath).arg(depth + 1));
                    }
                }
            }, Qt::QueuedConnection);
        }
    }
}

// Neue Methode: Lade Kinder beim Aufklappen
void HierarchicalDialog::loadChildrenOnExpand(QTreeWidgetItem *item)
{
    if (!item) return;
    
    // Prüfe ob bereits geladen
    bool childrenLoaded = item->data(0, Qt::UserRole + 2).toBool();
    if (childrenLoaded) return;
    
    // Prüfe Tiefe
    int depth = item->data(0, Qt::UserRole + 1).toInt();
    if (depth >= 20) return;
    
    QString path = item->data(0, Qt::UserRole).toString();
    if (path.isEmpty()) return;
    
    // Entferne Dummy Items
    for (int i = item->childCount() - 1; i >= 0; --i) {
        QTreeWidgetItem *child = item->child(i);
        if (child->data(0, Qt::UserRole + 3).toBool()) { // Ist Dummy?
            delete child;
        }
    }
    
    // Status anzeigen
    QTreeWidgetItem *loadingItem = new QTreeWidgetItem(item);
    loadingItem->setText(0, "🔄 Lade Unterverzeichnisse...");
    loadingItem->setDisabled(true);
    
    // Lade Kinder in separatem Thread
    QThread *worker = QThread::create([this, path, depth, item]() {
        loadDirectoryLevel(path, depth);
        
        QMetaObject::invokeMethod(this, [this, item]() {
            // Entferne Loading-Item
            for (int i = item->childCount() - 1; i >= 0; --i) {
                QTreeWidgetItem *child = item->child(i);
                if (child->text(0).contains("🔄")) {
                    delete child;
                }
            }
            
            // Markiere als geladen
            item->setData(0, Qt::UserRole + 2, true);
        }, Qt::QueuedConnection);
    });
    
    worker->start();
}

// Debug version of deleteSelectedDuplicates() function
// This will help us understand why the delete function isn't working

#include <QDebug>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QStringList>
#include <QSet>

void MainWindow::deleteSelectedDuplicates()
{
    qDebug() << "\n=== DELETE SELECTED DUPLICATES DEBUG ===";
    
    // Step 1: Check if there are selected items
    QList<QTableWidgetItem*> selectedItems = resultsTable->selectedItems();
    qDebug() << "🔍 Selected items count:" << selectedItems.size();
    
    if (selectedItems.isEmpty()) {
        qDebug() << "❌ No items selected - showing info dialog";
        QMessageBox::information(this, "Keine Auswahl", 
            "Bitte wählen Sie die zu löschenden Dateien aus.");
        return;
    }
    
    // Step 2: Analyze table structure
    qDebug() << "📊 Table info:";
    qDebug() << "   Rows:" << resultsTable->rowCount();
    qDebug() << "   Columns:" << resultsTable->columnCount();
    qDebug() << "   Selected items:" << selectedItems.size();
    
    // Step 3: Debug each selected item
    for (int i = 0; i < selectedItems.size() && i < 5; ++i) {
        QTableWidgetItem *item = selectedItems[i];
        qDebug() << QString("🔍 Item %1: Row=%2, Col=%3, Text='%4'")
                    .arg(i).arg(item->row()).arg(item->column()).arg(item->text());
    }
    
    // Step 4: Get unique rows
    QSet<int> selectedRows;
    for (QTableWidgetItem *item : selectedItems) {
        selectedRows.insert(item->row());
    }
    qDebug() << "🎯 Unique rows selected:" << selectedRows.size() << "rows";
    
    // Step 5: Extract file paths and debug each row
    QStringList selectedFiles;
    QStringList originalFiles;
    
    for (int row : selectedRows) {
        qDebug() << QString("\n📋 Processing row %1:").arg(row);
        
        // Check each column
        for (int col = 0; col < resultsTable->columnCount(); ++col) {
            QTableWidgetItem *item = resultsTable->item(row, col);
            if (item) {
                qDebug() << QString("   Col %1: '%2'").arg(col).arg(item->text());
                if (!item->data(Qt::UserRole).toString().isEmpty()) {
                    qDebug() << QString("   UserRole data: '%1'").arg(item->data(Qt::UserRole).toString());
                }
            } else {
                qDebug() << QString("   Col %1: NULL").arg(col);
            }
        }
        
        // Try to extract path from different columns
        QTableWidgetItem *pathItem = resultsTable->item(row, 2); // Path column
        QTableWidgetItem *statusItem = resultsTable->item(row, 0); // Status column
        
        if (pathItem) {
            QString filePath = pathItem->data(Qt::UserRole).toString();
            if (filePath.isEmpty()) {
                filePath = pathItem->text(); // Fallback to visible text
            }
            
            qDebug() << QString("📁 Path from row %1: '%2'").arg(row).arg(filePath);
            
            if (!filePath.isEmpty()) {
                selectedFiles.append(filePath);
                
                // Check if it's an original file
                bool isOriginal = false;
                if (statusItem) {
                    isOriginal = statusItem->data(Qt::UserRole + 1).toBool();
                    qDebug() << QString("🏷️ Is original: %1").arg(isOriginal ? "YES" : "NO");
                }
                
                if (isOriginal) {
                    originalFiles.append(filePath);
                }
            } else {
                qDebug() << QString("⚠️ Row %1: Empty file path!").arg(row);
            }
        } else {
            qDebug() << QString("❌ Row %1: No path item in column 2!").arg(row);
        }
    }
    
    qDebug() << "\n📋 EXTRACTION RESULTS:";
    qDebug() << "   Selected files:" << selectedFiles.size();
    qDebug() << "   Original files:" << originalFiles.size();
    
    // Debug each file path
    for (int i = 0; i < selectedFiles.size(); ++i) {
        qDebug() << QString("   File %1: '%2'").arg(i+1).arg(selectedFiles[i]);
    }
    
    if (selectedFiles.isEmpty()) {
        qDebug() << "❌ No valid file paths found!";
        QMessageBox::information(this, "Keine gültigen Dateien", 
            "Die ausgewählten Zeilen enthalten keine gültigen Dateipfade.");
        return;
    }
    
    // Step 6: Create debug dialog info
    qDebug() << "\n🚀 Creating DuplicateDeleteDialog...";
    qDebug() << "   Files to process:" << selectedFiles;
    qDebug() << "   Original files to protect:" << originalFiles;
    
    // Show deletion dialog
    DuplicateDeleteDialog *dialog = new DuplicateDeleteDialog(this);
    dialog->setPresetManager(m_presetManager);
    dialog->setDeletionMode("Markierte Dateien");
    dialog->setFileList(selectedFiles, originalFiles);
    
    connect(dialog, &DuplicateDeleteDialog::deletionCompleted,
            this, &MainWindow::onDeletionCompleted);
    
    qDebug() << "✅ Dialog created and configured. Showing dialog...";
    dialog->exec();
    qDebug() << "=== DELETE FUNCTION DEBUG END ===\n";
}

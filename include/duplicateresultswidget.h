#ifndef DUPLICATERESULTSWIDGET_H
#define DUPLICATERESULTSWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QProgressBar>
#include <QMenu>
#include <QHeaderView>
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>
#include <QHash>
#include <QFileIconProvider>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

// Import structs from scanner
#include "scanner.h"

// Forward declarations
class QAction;
enum OriginalFileRule
{
    KEEP_OLDEST,        // Älteste Datei als Original behalten
    KEEP_NEWEST,        // Jüngste Datei als Original behalten (Standard)
    KEEP_LARGEST,       // Größte Datei als Original behalten
    KEEP_SMALLEST,      // Kleinste Datei als Original behalten
    KEEP_BY_NAME_PATTERN, // Nach Dateinamen-Muster (Presets) behalten
    KEEP_MANUAL_SELECTION // Manuell ausgewählte Datei behalten
};

class DuplicateResultsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DuplicateResultsWidget(QWidget *parent = nullptr);
    ~DuplicateResultsWidget();

    // Display management
    void displayDuplicateResults(const DuplicateGroups &groups);
    void clearResults();
    void refreshDisplay();

    // Selection management
    void selectAllDuplicates();
    void deselectAllDuplicates();
    void selectDuplicatesInGroup(int groupIndex);

    // File operations
    void deleteSelectedDuplicates();
    void deleteAllDuplicates();
    void moveSelectedToTrash();
    void openSelectedFiles();
    void showSelectedInExplorer();
    
    // 🎯 NEUE FUNKTIONEN: Erweiterte Duplikat-Verwaltung
    void removeSelectedFromList();
    void removeAllDuplicatesFromList();
    void markAsOriginal();
    void excludeFromDeletion();
    void restoreToList();

    // Configuration
    void setOriginalFileRule(OriginalFileRule rule);
    void setAutoSelectDuplicates(bool enabled);
    void setColorCoding(bool enabled);

    // 📤 EXPORT-FUNKTIONEN (CSV / JSON / Markdown)
    bool exportToCsv(const QString &filePath);
    bool exportToJson(const QString &filePath);
    bool exportToMarkdown(const QString &filePath);
    QString generateMarkdownTable() const; // Für Zwischenablage / direkte Nutzung

signals:
    void duplicatesDeleted(const QStringList &deletedFiles);
    void duplicatesMovedToTrash(const QStringList &trashedFiles);
    void filesOpened(const QStringList &filePaths);
    void statusMessage(const QString &message);
    void selectionChanged(int selectedDuplicates, qint64 selectedSize);

private slots:
    void onItemChanged(QTableWidgetItem *item);
    void onItemDoubleClicked(QTableWidgetItem *item);
    void showContextMenu(const QPoint &pos);
    void updateSelection();
    void toggleGroupSelection();
    void previewSelectedFile();

private:
    void setupUI();
    void setupTable();
    void setupControls();
    void setupContextMenu();

    void populateTable(const DuplicateGroups &groups);
    QTableWidgetItem *createFileItem(const FileInfo &file, bool isOriginal);
    void applyColorCoding();
    void updateStatistics();
    void determineOriginalFiles();
    FileInfo determineOriginalByRule(const QList<FileInfo> &files, OriginalFileRule rule); // 🎯 NEUE HILFSFUNKTION
    FileInfo selectBestFileByPattern(const QList<FileInfo> &files) const; // 🎯 NEUE METHODE
    int calculateFileQualityScore(const FileInfo &file) const; // 🎯 NEUE METHODE
    void populateRow(int row, const FileInfo &file, bool isOriginal);
    
    // 🎯 NEUE VERZEICHNIS-BASIERTE ORIGINAL-AUSWAHL
    FileInfo selectDirectoryBasedOriginal(const DuplicateGroup &group);

    QString formatFileSize(qint64 bytes) const;
    QString formatDateTime(qint64 timestamp) const;
    QIcon getFileTypeIcon(const QString &filePath) const;

private:
    // UI Components
    QVBoxLayout *mainLayout;

    // Statistics panel
    QGroupBox *statsGroup;
    QLabel *totalFilesLabel;
    QLabel *duplicateGroupsLabel;
    QLabel *duplicateSizeLabel;
    QLabel *selectedCountLabel;
    QLabel *selectedSizeLabel;
    QProgressBar *deletionProgress;

    // Configuration panel
    QGroupBox *configGroup;
    QComboBox *originalRuleCombo;
    QCheckBox *autoSelectCheck;
    QCheckBox *colorCodingCheck;
    QPushButton *refreshBtn;

    // Results table
    QGroupBox *resultsGroup;
    QTableWidget *resultsTable;

    // Action buttons
    QWidget *buttonWidget;
    QPushButton *selectAllBtn;
    QPushButton *deselectAllBtn;
    QPushButton *deleteBtn;
    QPushButton *moveToTrashBtn;
    QPushButton *openBtn;
    QPushButton *exploreBtn;

    // Context menu
    QMenu *contextMenu;
    QAction *openAction;
    QAction *exploreAction;
    QAction *deleteAction;
    QAction *trashAction;
    QAction *selectGroupAction;
    QAction *deselectGroupAction;
    QAction *previewAction;
    
    // 🎯 NEUE CONTEXT-MENU-AKTIONEN
    QAction *removeFromListAction;      // "Aus Liste entfernen"
    QAction *removeAllAction;           // "Alle Duplikate entfernen" 
    QAction *markOriginalAction;        // "Als Original markieren"
    QAction *excludeFromDeletionAction; // "Von Löschung ausschließen"
    QAction *restoreToListAction;       // "Zur Liste hinzufügen"
    QMenu *originalRuleMenu;            // Submenu für Original-Regeln
    QAction *useOldestAction;           // "Älteste behalten"
    QAction *useNewestAction;           // "Jüngste behalten"
    QAction *useLargestAction;          // "Größte behalten"
    QAction *useNamePatternAction;      // "Nach Name-Pattern behalten"
    // Export Actions
    QAction *exportCsvAction;
    QAction *exportJsonAction;
    QAction *exportMarkdownAction;

    // Export Buttons (optional separate UI section)
    QPushButton *exportCsvBtn;
    QPushButton *exportJsonBtn;
    QPushButton *exportMarkdownBtn;

    // Data
    DuplicateGroups currentResults;
    QHash<int, DuplicateGroup> groupLookup; // Row -> Group mapping
    OriginalFileRule currentRule;

    // State
    bool autoSelectDuplicates;
    bool colorCodingEnabled;
    int selectedDuplicateCount;
    qint64 selectedDuplicateSize;

    // Colors for original/duplicate files
    static const QColor ORIGINAL_COLOR;  // Yellow background
    static const QColor DUPLICATE_COLOR; // Light green background
    static const QColor SELECTED_COLOR;  // Blue tint

    // 🔧 Hilfsfunktionen für Export
    QStringList buildCsvLines() const;
    QJsonArray buildJsonArray() const; // Vorwärtsdeklaration reicht nicht, include <QJsonArray> nötig oben
};

#endif // DUPLICATERESULTSWIDGET_H

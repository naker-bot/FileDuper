#ifndef NETWORKPRESETDIALOG_H
#define NETWORKPRESETDIALOG_H

#include <QDialog>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGroupBox>
#include <QListWidget>
#include <QTreeWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>
#include <QSpinBox>
#include <QTabWidget>
#include <QSplitter>
#include <QToolButton>
#include <QMenu>
#include <QAction>
#include <QProgressBar>
#include <QFrame>

#include "networkpresetmanager.h"

class NetworkPresetListWidget : public QTreeWidget
{
    Q_OBJECT

public:
    explicit NetworkPresetListWidget(QWidget *parent = nullptr);
    
    void setPresetManager(NetworkPresetManager *manager);
    void refreshPresets();
    void selectPreset(const QString &name);
    QString getSelectedPreset() const;

signals:
    void presetSelected(const QString &name);
    void presetDoubleClicked(const QString &name);
    void presetContextMenu(const QString &name, const QPoint &pos);

private slots:
    void onItemSelectionChanged();
    void onItemDoubleClicked(QTreeWidgetItem *item, int column);
    void onContextMenuRequested(const QPoint &pos);

private:
    void setupColumns();
    void addPresetItem(const NetworkPreset &preset, QTreeWidgetItem *parent = nullptr);
    void updatePresetItem(QTreeWidgetItem *item, const NetworkPreset &preset);
    QTreeWidgetItem *findPresetItem(const QString &name) const;
    QTreeWidgetItem *getOrCreateCategoryItem(const QString &category);
    
    NetworkPresetManager *m_presetManager;
    QMap<QString, QTreeWidgetItem*> m_categoryItems;
};

class NetworkPresetEditor : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkPresetEditor(QWidget *parent = nullptr);
    
    void setPresetManager(NetworkPresetManager *manager);
    void setPreset(const NetworkPreset &preset);
    NetworkPreset getPreset() const;
    void clearEditor();
    bool validateInput();
    QStringList getValidationErrors() const;

signals:
    void presetChanged();
    void validationChanged(bool isValid);

private slots:
    void onInputChanged();
    void onCidrChanged();
    void onAddTag();
    void onRemoveTag();
    void onColorButtonClicked();

private:
    void setupUI();
    void setupValidation();
    void updateValidation();
    void updateColorButton();
    void addTagToList(const QString &tag);
    
    NetworkPresetManager *m_presetManager;
    
    // Basic information
    QLineEdit *m_nameEdit;
    QLineEdit *m_cidrEdit;
    QTextEdit *m_descriptionEdit;
    QComboBox *m_categoryCombo;
    QLineEdit *m_interfaceEdit;
    
    // Tags
    QLineEdit *m_tagEdit;
    QPushButton *m_addTagButton;
    QListWidget *m_tagsList;
    QPushButton *m_removeTagButton;
    
    // Appearance
    QPushButton *m_colorButton;
    QString m_selectedColor;
    
    // Options
    QCheckBox *m_readOnlyCheck;
    
    // Validation
    QLabel *m_validationLabel;
    bool m_isValid;
    QStringList m_validationErrors;
};

class NetworkPresetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NetworkPresetDialog(QWidget *parent = nullptr);
    
    void setPresetManager(NetworkPresetManager *manager);
    
    // Dialog modes
    void setAddMode();
    void setEditMode(const QString &presetName);
    void setViewMode();
    
    // Result
    NetworkPreset getResult() const;

private slots:
    void onPresetSelected(const QString &name);
    void onPresetDoubleClicked(const QString &name);
    void onPresetContextMenu(const QString &name, const QPoint &pos);
    void onAddPreset();
    void onEditPreset();
    void onDeletePreset();
    void onDuplicatePreset();
    void onExportPreset();
    void onImportPresets();
    void onRefreshList();
    void onSetAsActive();
    void onAddToFavorites();
    void onRemoveFromFavorites();
    void onCreateGroup();
    void onManageGroups();
    void onValidationChanged(bool isValid);
    void onAccept();
    void onReject();

private:
    void setupUI();
    void setupToolbar();
    void setupContextMenu();
    void connectSignals();
    void updateButtonStates();
    void updateWindowTitle();
    void loadPresetForEditing(const QString &name);
    bool saveCurrentPreset();
    void showPresetDetails(const QString &name);
    
    NetworkPresetManager *m_presetManager;
    
    // UI Components
    QTabWidget *m_tabWidget;
    QSplitter *m_mainSplitter;
    
    // Preset list tab
    NetworkPresetListWidget *m_presetList;
    QToolButton *m_addButton;
    QToolButton *m_editButton;
    QToolButton *m_deleteButton;
    QToolButton *m_duplicateButton;
    QToolButton *m_refreshButton;
    QMenu *m_moreMenu;
    
    // Editor tab
    NetworkPresetEditor *m_presetEditor;
    QPushButton *m_saveButton;
    QPushButton *m_cancelButton;
    QPushButton *m_applyButton;
    
    // Status
    QLabel *m_statusLabel;
    QProgressBar *m_progressBar;
    
    // Context menu
    QMenu *m_contextMenu;
    QAction *m_setActiveAction;
    QAction *m_editAction;
    QAction *m_deleteAction;
    QAction *m_duplicateAction;
    QAction *m_favoriteAction;
    QAction *m_exportAction;
    
    // State
    enum DialogMode { AddMode, EditMode, ViewMode };
    DialogMode m_mode;
    QString m_currentPresetName;
    bool m_hasUnsavedChanges;
};

class NetworkPresetQuickSelector : public QWidget
{
    Q_OBJECT

public:
    explicit NetworkPresetQuickSelector(QWidget *parent = nullptr);
    
    void setPresetManager(NetworkPresetManager *manager);
    void refreshPresets();
    QString getSelectedPreset() const;
    void setSelectedPreset(const QString &name);

signals:
    void presetSelected(const QString &name);
    void managePresetsRequested();

private slots:
    void onPresetChanged();
    void onManageButtonClicked();
    void onQuickAddRequested();

private:
    void setupUI();
    void updateComboBox();
    
    NetworkPresetManager *m_presetManager;
    QComboBox *m_presetCombo;
    QPushButton *m_manageButton;
    QPushButton *m_quickAddButton;
};

#endif // NETWORKPRESETDIALOG_H
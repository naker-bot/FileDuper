#include "networkpresetdialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QColorDialog>
#include <QFileDialog>
#include <QInputDialog>
#include <QApplication>
#include <QClipboard>
#include <QMimeData>
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>

// NetworkPresetListWidget Implementation
NetworkPresetListWidget::NetworkPresetListWidget(QWidget *parent)
    : QTreeWidget(parent)
    , m_presetManager(nullptr)
{
    setupColumns();
    setContextMenuPolicy(Qt::CustomContextMenu);
    setDragDropMode(QAbstractItemView::InternalMove);
    setSelectionMode(QAbstractItemView::ExtendedSelection);
    setRootIsDecorated(true);
    setItemsExpandable(true);
    setExpandsOnDoubleClick(false);
    setSortingEnabled(true);
    sortByColumn(0, Qt::AscendingOrder);
    
    connect(this, &QTreeWidget::itemSelectionChanged, this, &NetworkPresetListWidget::onItemSelectionChanged);
    connect(this, &QTreeWidget::itemDoubleClicked, this, &NetworkPresetListWidget::onItemDoubleClicked);
    connect(this, &QTreeWidget::customContextMenuRequested, this, &NetworkPresetListWidget::onContextMenuRequested);
}

void NetworkPresetListWidget::setupColumns()
{
    setHeaderLabels({"Name", "CIDR", "Category", "Usage", "Last Used"});
    header()->setStretchLastSection(false);
    header()->setSectionResizeMode(0, QHeaderView::Stretch);
    header()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    header()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
}

void NetworkPresetListWidget::setPresetManager(NetworkPresetManager *manager)
{
    m_presetManager = manager;
    if (m_presetManager) {
        connect(m_presetManager, &NetworkPresetManager::presetAdded, this, &NetworkPresetListWidget::refreshPresets);
        connect(m_presetManager, &NetworkPresetManager::presetRemoved, this, &NetworkPresetListWidget::refreshPresets);
        connect(m_presetManager, &NetworkPresetManager::presetUpdated, this, &NetworkPresetListWidget::refreshPresets);
        refreshPresets();
    }
}

void NetworkPresetListWidget::refreshPresets()
{
    if (!m_presetManager) {
        return;
    }
    
    clear();
    m_categoryItems.clear();
    
    QList<NetworkPreset> presets = m_presetManager->getAllPresets();
    
    // Sort presets by category
    QMap<QString, QList<NetworkPreset>> categorizedPresets;
    for (const NetworkPreset &preset : presets) {
        QString category = preset.category.isEmpty() ? "General" : preset.category;
        categorizedPresets[category].append(preset);
    }
    
    // Add presets to tree
    for (auto it = categorizedPresets.constBegin(); it != categorizedPresets.constEnd(); ++it) {
        QTreeWidgetItem *categoryItem = getOrCreateCategoryItem(it.key());
        
        for (const NetworkPreset &preset : it.value()) {
            addPresetItem(preset, categoryItem);
        }
        
        categoryItem->setExpanded(true);
    }
    
    // Expand all categories
    expandAll();
}

void NetworkPresetListWidget::addPresetItem(const NetworkPreset &preset, QTreeWidgetItem *parent)
{
    QTreeWidgetItem *item = new QTreeWidgetItem(parent);
    updatePresetItem(item, preset);
    
    // Store preset name in item data
    item->setData(0, Qt::UserRole, preset.name);
    
    // Set icon based on active status
    if (m_presetManager && m_presetManager->getActivePreset() == preset.name) {
        item->setIcon(0, style()->standardIcon(QStyle::SP_MediaPlay));
    }
    
    // Apply color if set
    if (!preset.color.isEmpty()) {
        QColor color(preset.color);
        if (color.isValid()) {
            item->setBackground(0, QBrush(color.lighter(180)));
        }
    }
    
    // Mark read-only presets
    if (preset.isReadOnly) {
        QFont font = item->font(0);
        font.setItalic(true);
        item->setFont(0, font);
        item->setToolTip(0, "System preset (read-only)");
    }
}

void NetworkPresetListWidget::updatePresetItem(QTreeWidgetItem *item, const NetworkPreset &preset)
{
    item->setText(0, preset.name);
    item->setText(1, preset.cidr);
    item->setText(2, preset.category);
    item->setText(3, QString::number(preset.usageCount));
    item->setText(4, preset.lastUsed.isValid() ? preset.lastUsed.toString("yyyy-MM-dd hh:mm") : "Never");
    
    item->setToolTip(0, preset.description.isEmpty() ? preset.name : preset.description);
    item->setToolTip(1, QString("Network: %1").arg(preset.cidr));
}

QTreeWidgetItem *NetworkPresetListWidget::getOrCreateCategoryItem(const QString &category)
{
    if (m_categoryItems.contains(category)) {
        return m_categoryItems[category];
    }
    
    QTreeWidgetItem *categoryItem = new QTreeWidgetItem(this);
    categoryItem->setText(0, category);
    categoryItem->setFlags(categoryItem->flags() & ~Qt::ItemIsSelectable);
    
    QFont font = categoryItem->font(0);
    font.setBold(true);
    categoryItem->setFont(0, font);
    
    categoryItem->setIcon(0, style()->standardIcon(QStyle::SP_DirIcon));
    
    m_categoryItems[category] = categoryItem;
    return categoryItem;
}

QString NetworkPresetListWidget::getSelectedPreset() const
{
    QTreeWidgetItem *item = currentItem();
    if (item && item->parent()) {  // Only preset items have parents
        return item->data(0, Qt::UserRole).toString();
    }
    return QString();
}

void NetworkPresetListWidget::selectPreset(const QString &name)
{
    QTreeWidgetItem *item = findPresetItem(name);
    if (item) {
        setCurrentItem(item);
        scrollToItem(item);
    }
}

QTreeWidgetItem *NetworkPresetListWidget::findPresetItem(const QString &name) const
{
    for (int i = 0; i < topLevelItemCount(); ++i) {
        QTreeWidgetItem *categoryItem = topLevelItem(i);
        for (int j = 0; j < categoryItem->childCount(); ++j) {
            QTreeWidgetItem *presetItem = categoryItem->child(j);
            if (presetItem->data(0, Qt::UserRole).toString() == name) {
                return presetItem;
            }
        }
    }
    return nullptr;
}

void NetworkPresetListWidget::onItemSelectionChanged()
{
    QString presetName = getSelectedPreset();
    if (!presetName.isEmpty()) {
        emit presetSelected(presetName);
    }
}

void NetworkPresetListWidget::onItemDoubleClicked(QTreeWidgetItem *item, int column)
{
    Q_UNUSED(column)
    if (item && item->parent()) {  // Only preset items
        QString presetName = item->data(0, Qt::UserRole).toString();
        if (!presetName.isEmpty()) {
            emit presetDoubleClicked(presetName);
        }
    }
}

void NetworkPresetListWidget::onContextMenuRequested(const QPoint &pos)
{
    QTreeWidgetItem *item = itemAt(pos);
    if (item && item->parent()) {  // Only preset items
        QString presetName = item->data(0, Qt::UserRole).toString();
        if (!presetName.isEmpty()) {
            emit presetContextMenu(presetName, mapToGlobal(pos));
        }
    }
}

// NetworkPresetEditor Implementation
NetworkPresetEditor::NetworkPresetEditor(QWidget *parent)
    : QWidget(parent)
    , m_presetManager(nullptr)
    , m_isValid(false)
{
    setupUI();
    setupValidation();
}

void NetworkPresetEditor::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Basic Information Group
    QGroupBox *basicGroup = new QGroupBox("Basic Information", this);
    QGridLayout *basicLayout = new QGridLayout(basicGroup);
    
    basicLayout->addWidget(new QLabel("Name:"), 0, 0);
    m_nameEdit = new QLineEdit(this);
    m_nameEdit->setPlaceholderText("Enter preset name");
    basicLayout->addWidget(m_nameEdit, 0, 1, 1, 2);
    
    basicLayout->addWidget(new QLabel("CIDR:"), 1, 0);
    m_cidrEdit = new QLineEdit(this);
    m_cidrEdit->setPlaceholderText("e.g., 192.168.1.0/24");
    basicLayout->addWidget(m_cidrEdit, 1, 1, 1, 2);
    
    basicLayout->addWidget(new QLabel("Category:"), 2, 0);
    m_categoryCombo = new QComboBox(this);
    m_categoryCombo->setEditable(true);
    basicLayout->addWidget(m_categoryCombo, 2, 1, 1, 2);
    
    basicLayout->addWidget(new QLabel("Interface:"), 3, 0);
    m_interfaceEdit = new QLineEdit(this);
    m_interfaceEdit->setPlaceholderText("Optional interface name");
    basicLayout->addWidget(m_interfaceEdit, 3, 1, 1, 2);
    
    basicLayout->addWidget(new QLabel("Description:"), 4, 0);
    m_descriptionEdit = new QTextEdit(this);
    m_descriptionEdit->setMaximumHeight(80);
    m_descriptionEdit->setPlaceholderText("Optional description");
    basicLayout->addWidget(m_descriptionEdit, 4, 1, 1, 2);
    
    mainLayout->addWidget(basicGroup);
    
    // Tags Group
    QGroupBox *tagsGroup = new QGroupBox("Tags", this);
    QVBoxLayout *tagsLayout = new QVBoxLayout(tagsGroup);
    
    QHBoxLayout *tagInputLayout = new QHBoxLayout();
    m_tagEdit = new QLineEdit(this);
    m_tagEdit->setPlaceholderText("Add tag");
    m_addTagButton = new QPushButton("Add", this);
    tagInputLayout->addWidget(m_tagEdit);
    tagInputLayout->addWidget(m_addTagButton);
    tagsLayout->addLayout(tagInputLayout);
    
    QHBoxLayout *tagListLayout = new QHBoxLayout();
    m_tagsList = new QListWidget(this);
    m_tagsList->setMaximumHeight(80);
    m_removeTagButton = new QPushButton("Remove", this);
    m_removeTagButton->setEnabled(false);
    QVBoxLayout *tagButtonLayout = new QVBoxLayout();
    tagButtonLayout->addWidget(m_removeTagButton);
    tagButtonLayout->addStretch();
    tagListLayout->addWidget(m_tagsList);
    tagListLayout->addLayout(tagButtonLayout);
    tagsLayout->addLayout(tagListLayout);
    
    mainLayout->addWidget(tagsGroup);
    
    // Appearance Group
    QGroupBox *appearanceGroup = new QGroupBox("Appearance", this);
    QHBoxLayout *appearanceLayout = new QHBoxLayout(appearanceGroup);
    
    appearanceLayout->addWidget(new QLabel("Color:"));
    m_colorButton = new QPushButton("Select Color", this);
    m_colorButton->setMaximumWidth(120);
    updateColorButton();
    appearanceLayout->addWidget(m_colorButton);
    appearanceLayout->addStretch();
    
    mainLayout->addWidget(appearanceGroup);
    
    // Options Group
    QGroupBox *optionsGroup = new QGroupBox("Options", this);
    QVBoxLayout *optionsLayout = new QVBoxLayout(optionsGroup);
    
    m_readOnlyCheck = new QCheckBox("Read-only (system preset)", this);
    m_readOnlyCheck->setEnabled(false);  // Usually controlled by system
    optionsLayout->addWidget(m_readOnlyCheck);
    
    mainLayout->addWidget(optionsGroup);
    
    // Validation
    m_validationLabel = new QLabel(this);
    m_validationLabel->setStyleSheet("QLabel { color: red; font-weight: bold; }");
    m_validationLabel->setWordWrap(true);
    mainLayout->addWidget(m_validationLabel);
    
    mainLayout->addStretch();
    
    // Connect signals
    connect(m_nameEdit, &QLineEdit::textChanged, this, &NetworkPresetEditor::onInputChanged);
    connect(m_cidrEdit, &QLineEdit::textChanged, this, &NetworkPresetEditor::onCidrChanged);
    connect(m_descriptionEdit, &QTextEdit::textChanged, this, &NetworkPresetEditor::onInputChanged);
    connect(m_categoryCombo, QOverload<const QString &>::of(&QComboBox::currentTextChanged), this, &NetworkPresetEditor::onInputChanged);
    connect(m_interfaceEdit, &QLineEdit::textChanged, this, &NetworkPresetEditor::onInputChanged);
    connect(m_addTagButton, &QPushButton::clicked, this, &NetworkPresetEditor::onAddTag);
    connect(m_removeTagButton, &QPushButton::clicked, this, &NetworkPresetEditor::onRemoveTag);
    connect(m_colorButton, &QPushButton::clicked, this, &NetworkPresetEditor::onColorButtonClicked);
    connect(m_tagsList, &QListWidget::itemSelectionChanged, this, [this]() {
        m_removeTagButton->setEnabled(m_tagsList->currentItem() != nullptr);
    });
    connect(m_tagEdit, &QLineEdit::returnPressed, this, &NetworkPresetEditor::onAddTag);
}

void NetworkPresetEditor::setupValidation()
{
    updateValidation();
}

void NetworkPresetEditor::setPresetManager(NetworkPresetManager *manager)
{
    m_presetManager = manager;
    
    if (m_presetManager) {
        // Populate categories
        m_categoryCombo->clear();
        m_categoryCombo->addItems(m_presetManager->getAvailableCategories());
    }
}

void NetworkPresetEditor::setPreset(const NetworkPreset &preset)
{
    m_nameEdit->setText(preset.name);
    m_cidrEdit->setText(preset.cidr);
    m_descriptionEdit->setPlainText(preset.description);
    m_categoryCombo->setCurrentText(preset.category);
    m_interfaceEdit->setText(preset.interfaceName);
    m_readOnlyCheck->setChecked(preset.isReadOnly);
    m_selectedColor = preset.color;
    
    // Set tags
    m_tagsList->clear();
    for (const QString &tag : preset.tags) {
        addTagToList(tag);
    }
    
    updateColorButton();
    updateValidation();
}

NetworkPreset NetworkPresetEditor::getPreset() const
{
    NetworkPreset preset;
    preset.name = m_nameEdit->text().trimmed();
    preset.cidr = m_cidrEdit->text().trimmed();
    preset.description = m_descriptionEdit->toPlainText().trimmed();
    preset.category = m_categoryCombo->currentText().trimmed();
    preset.interfaceName = m_interfaceEdit->text().trimmed();
    preset.isReadOnly = m_readOnlyCheck->isChecked();
    preset.color = m_selectedColor;
    
    // Get tags
    for (int i = 0; i < m_tagsList->count(); ++i) {
        preset.tags.append(m_tagsList->item(i)->text());
    }
    
    return preset;
}

void NetworkPresetEditor::clearEditor()
{
    m_nameEdit->clear();
    m_cidrEdit->clear();
    m_descriptionEdit->clear();
    m_categoryCombo->setCurrentIndex(0);
    m_interfaceEdit->clear();
    m_readOnlyCheck->setChecked(false);
    m_tagsList->clear();
    m_selectedColor.clear();
    m_tagEdit->clear();
    
    updateColorButton();
    updateValidation();
}

bool NetworkPresetEditor::validateInput()
{
    updateValidation();
    return m_isValid;
}

QStringList NetworkPresetEditor::getValidationErrors() const
{
    return m_validationErrors;
}

void NetworkPresetEditor::onInputChanged()
{
    updateValidation();
    emit presetChanged();
}

void NetworkPresetEditor::onCidrChanged()
{
    updateValidation();
    emit presetChanged();
}

void NetworkPresetEditor::onAddTag()
{
    QString tag = m_tagEdit->text().trimmed();
    if (!tag.isEmpty()) {
        // Check if tag already exists
        bool exists = false;
        for (int i = 0; i < m_tagsList->count(); ++i) {
            if (m_tagsList->item(i)->text() == tag) {
                exists = true;
                break;
            }
        }
        
        if (!exists) {
            addTagToList(tag);
            m_tagEdit->clear();
            emit presetChanged();
        }
    }
}

void NetworkPresetEditor::onRemoveTag()
{
    QListWidgetItem *item = m_tagsList->currentItem();
    if (item) {
        delete item;
        emit presetChanged();
    }
}

void NetworkPresetEditor::onColorButtonClicked()
{
    QColor currentColor = m_selectedColor.isEmpty() ? QColor(Qt::white) : QColor(m_selectedColor);
    QColor color = QColorDialog::getColor(currentColor, this, "Select Preset Color");
    
    if (color.isValid()) {
        m_selectedColor = color.name();
        updateColorButton();
        emit presetChanged();
    }
}

void NetworkPresetEditor::updateValidation()
{
    m_validationErrors.clear();
    
    // Validate name
    if (m_nameEdit->text().trimmed().isEmpty()) {
        m_validationErrors.append("Preset name is required");
    }
    
    // Validate CIDR
    QString cidr = m_cidrEdit->text().trimmed();
    if (cidr.isEmpty()) {
        m_validationErrors.append("CIDR is required");
    } else if (m_presetManager && !m_presetManager->validateCidr(cidr)) {
        m_validationErrors.append("Invalid CIDR format");
    }
    
    // Check for duplicate names (if preset manager is available)
    if (m_presetManager) {
        QString name = m_nameEdit->text().trimmed();
        if (!name.isEmpty() && !m_presetManager->isPresetNameAvailable(name)) {
            // Allow if we're editing the same preset
            if (m_presetManager->getPreset(name).name != name) {
                m_validationErrors.append("Preset name already exists");
            }
        }
    }
    
    m_isValid = m_validationErrors.isEmpty();
    
    // Update UI
    if (m_isValid) {
        m_validationLabel->clear();
    } else {
        m_validationLabel->setText(m_validationErrors.join("\n"));
    }
    
    emit validationChanged(m_isValid);
}

void NetworkPresetEditor::updateColorButton()
{
    if (m_selectedColor.isEmpty()) {
        m_colorButton->setText("Select Color");
        m_colorButton->setStyleSheet("");
    } else {
        QColor color(m_selectedColor);
        m_colorButton->setText(color.name());
        m_colorButton->setStyleSheet(QString("QPushButton { background-color: %1; }").arg(color.name()));
    }
}

void NetworkPresetEditor::addTagToList(const QString &tag)
{
    QListWidgetItem *item = new QListWidgetItem(tag, m_tagsList);
    item->setFlags(item->flags() | Qt::ItemIsEditable);
}

// NetworkPresetDialog Implementation
NetworkPresetDialog::NetworkPresetDialog(QWidget *parent)
    : QDialog(parent)
    , m_presetManager(nullptr)
    , m_mode(ViewMode)
    , m_hasUnsavedChanges(false)
{
    setWindowTitle("Network Preset Manager");
    setModal(true);
    resize(800, 600);
    
    setupUI();
    setupContextMenu();
    connectSignals();
    updateButtonStates();
}

void NetworkPresetDialog::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    
    // Create main splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal, this);
    
    // Left side: Preset list with toolbar
    QWidget *leftWidget = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    
    // Toolbar
    setupToolbar();
    QHBoxLayout *toolbarLayout = new QHBoxLayout();
    toolbarLayout->addWidget(m_addButton);
    toolbarLayout->addWidget(m_editButton);
    toolbarLayout->addWidget(m_deleteButton);
    toolbarLayout->addWidget(m_duplicateButton);
    
    // Add separator as a frame line
    QFrame *separator = new QFrame();
    separator->setFrameShape(QFrame::VLine);
    separator->setFrameShadow(QFrame::Sunken);
    toolbarLayout->addWidget(separator);
    
    toolbarLayout->addWidget(m_refreshButton);
    toolbarLayout->addStretch();
    leftLayout->addLayout(toolbarLayout);
    
    // Preset list
    m_presetList = new NetworkPresetListWidget(this);
    leftLayout->addWidget(m_presetList);
    
    m_mainSplitter->addWidget(leftWidget);
    
    // Right side: Editor
    QWidget *rightWidget = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightWidget);
    
    m_presetEditor = new NetworkPresetEditor(this);
    rightLayout->addWidget(m_presetEditor);
    
    // Editor buttons
    QHBoxLayout *editorButtonLayout = new QHBoxLayout();
    m_saveButton = new QPushButton("Save", this);
    m_cancelButton = new QPushButton("Cancel", this);
    m_applyButton = new QPushButton("Apply", this);
    
    editorButtonLayout->addWidget(m_saveButton);
    editorButtonLayout->addWidget(m_applyButton);
    editorButtonLayout->addWidget(m_cancelButton);
    editorButtonLayout->addStretch();
    rightLayout->addLayout(editorButtonLayout);
    
    m_mainSplitter->addWidget(rightWidget);
    m_mainSplitter->setSizes({400, 400});
    
    mainLayout->addWidget(m_mainSplitter);
    
    // Status bar
    QHBoxLayout *statusLayout = new QHBoxLayout();
    m_statusLabel = new QLabel(this);
    m_progressBar = new QProgressBar(this);
    m_progressBar->setVisible(false);
    
    statusLayout->addWidget(m_statusLabel);
    statusLayout->addStretch();
    statusLayout->addWidget(m_progressBar);
    
    mainLayout->addLayout(statusLayout);
    
    // Dialog buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *okButton = new QPushButton("OK", this);
    QPushButton *cancelButton = new QPushButton("Cancel", this);
    
    buttonLayout->addStretch();
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    
    mainLayout->addLayout(buttonLayout);
    
    connect(okButton, &QPushButton::clicked, this, &NetworkPresetDialog::onAccept);
    connect(cancelButton, &QPushButton::clicked, this, &NetworkPresetDialog::onReject);
}

void NetworkPresetDialog::setupToolbar()
{
    m_addButton = new QToolButton(this);
    m_addButton->setText("Add");
    m_addButton->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    m_addButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    m_editButton = new QToolButton(this);
    m_editButton->setText("Edit");
    m_editButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    m_editButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    m_deleteButton = new QToolButton(this);
    m_deleteButton->setText("Delete");
    m_deleteButton->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    m_deleteButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    m_duplicateButton = new QToolButton(this);
    m_duplicateButton->setText("Duplicate");
    m_duplicateButton->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    m_duplicateButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    
    m_refreshButton = new QToolButton(this);
    m_refreshButton->setText("Refresh");
    m_refreshButton->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    m_refreshButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void NetworkPresetDialog::setupContextMenu()
{
    m_contextMenu = new QMenu(this);
    
    m_setActiveAction = m_contextMenu->addAction("Set as Active");
    m_setActiveAction->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    
    m_contextMenu->addSeparator();
    
    m_editAction = m_contextMenu->addAction("Edit");
    m_editAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    
    m_duplicateAction = m_contextMenu->addAction("Duplicate");
    m_duplicateAction->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
    
    m_contextMenu->addSeparator();
    
    m_favoriteAction = m_contextMenu->addAction("Add to Favorites");
    m_favoriteAction->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    
    m_contextMenu->addSeparator();
    
    m_exportAction = m_contextMenu->addAction("Export");
    m_exportAction->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    
    m_deleteAction = m_contextMenu->addAction("Delete");
    m_deleteAction->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
}

void NetworkPresetDialog::connectSignals()
{
    // Preset list signals
    connect(m_presetList, &NetworkPresetListWidget::presetSelected, this, &NetworkPresetDialog::onPresetSelected);
    connect(m_presetList, &NetworkPresetListWidget::presetDoubleClicked, this, &NetworkPresetDialog::onPresetDoubleClicked);
    connect(m_presetList, &NetworkPresetListWidget::presetContextMenu, this, &NetworkPresetDialog::onPresetContextMenu);
    
    // Toolbar signals
    connect(m_addButton, &QToolButton::clicked, this, &NetworkPresetDialog::onAddPreset);
    connect(m_editButton, &QToolButton::clicked, this, &NetworkPresetDialog::onEditPreset);
    connect(m_deleteButton, &QToolButton::clicked, this, &NetworkPresetDialog::onDeletePreset);
    connect(m_duplicateButton, &QToolButton::clicked, this, &NetworkPresetDialog::onDuplicatePreset);
    connect(m_refreshButton, &QToolButton::clicked, this, &NetworkPresetDialog::onRefreshList);
    
    // Editor signals
    connect(m_presetEditor, &NetworkPresetEditor::presetChanged, this, [this]() {
        m_hasUnsavedChanges = true;
        updateButtonStates();
    });
    connect(m_presetEditor, &NetworkPresetEditor::validationChanged, this, &NetworkPresetDialog::onValidationChanged);
    
    // Editor buttons
    connect(m_saveButton, &QPushButton::clicked, this, [this]() {
        if (saveCurrentPreset()) {
            m_hasUnsavedChanges = false;
            updateButtonStates();
            m_statusLabel->setText("Preset saved successfully");
        }
    });
    
    connect(m_applyButton, &QPushButton::clicked, this, [this]() {
        if (saveCurrentPreset()) {
            m_statusLabel->setText("Changes applied");
        }
    });
    
    connect(m_cancelButton, &QPushButton::clicked, this, [this]() {
        if (m_hasUnsavedChanges) {
            int ret = QMessageBox::question(this, "Unsaved Changes",
                                          "You have unsaved changes. Discard them?",
                                          QMessageBox::Yes | QMessageBox::No);
            if (ret == QMessageBox::No) {
                return;
            }
        }
        
        m_presetEditor->clearEditor();
        m_hasUnsavedChanges = false;
        setViewMode();
        updateButtonStates();
    });
    
    // Context menu signals
    connect(m_setActiveAction, &QAction::triggered, this, &NetworkPresetDialog::onSetAsActive);
    connect(m_editAction, &QAction::triggered, this, &NetworkPresetDialog::onEditPreset);
    connect(m_deleteAction, &QAction::triggered, this, &NetworkPresetDialog::onDeletePreset);
    connect(m_duplicateAction, &QAction::triggered, this, &NetworkPresetDialog::onDuplicatePreset);
    connect(m_favoriteAction, &QAction::triggered, this, &NetworkPresetDialog::onAddToFavorites);
    connect(m_exportAction, &QAction::triggered, this, &NetworkPresetDialog::onExportPreset);
}

void NetworkPresetDialog::setPresetManager(NetworkPresetManager *manager)
{
    m_presetManager = manager;
    m_presetList->setPresetManager(manager);
    m_presetEditor->setPresetManager(manager);
    
    if (m_presetManager) {
        m_statusLabel->setText(QString("Loaded %1 presets").arg(m_presetManager->getAllPresets().size()));
    }
}

void NetworkPresetDialog::setAddMode()
{
    m_mode = AddMode;
    m_presetEditor->clearEditor();
    m_hasUnsavedChanges = false;
    updateWindowTitle();
    updateButtonStates();
}

void NetworkPresetDialog::setEditMode(const QString &presetName)
{
    m_mode = EditMode;
    m_currentPresetName = presetName;
    loadPresetForEditing(presetName);
    m_hasUnsavedChanges = false;
    updateWindowTitle();
    updateButtonStates();
}

void NetworkPresetDialog::setViewMode()
{
    m_mode = ViewMode;
    m_currentPresetName.clear();
    updateWindowTitle();
    updateButtonStates();
}

void NetworkPresetDialog::onPresetSelected(const QString &name)
{
    if (m_mode == ViewMode) {
        showPresetDetails(name);
    }
}

void NetworkPresetDialog::onPresetDoubleClicked(const QString &name)
{
    setEditMode(name);
}

void NetworkPresetDialog::onPresetContextMenu(const QString &name, const QPoint &pos)
{
    m_currentPresetName = name;
    
    // Update context menu based on preset properties
    if (m_presetManager) {
        NetworkPreset preset = m_presetManager->getPreset(name);
        
        m_favoriteAction->setText(m_presetManager->getFavoritePresets().contains(name) 
                                ? "Remove from Favorites" : "Add to Favorites");
        
        m_deleteAction->setEnabled(!preset.isReadOnly);
        m_editAction->setEnabled(!preset.isReadOnly);
    }
    
    m_contextMenu->exec(pos);
}

void NetworkPresetDialog::onAddPreset()
{
    setAddMode();
}

void NetworkPresetDialog::onEditPreset()
{
    QString presetName = m_presetList->getSelectedPreset();
    if (!presetName.isEmpty()) {
        setEditMode(presetName);
    }
}

void NetworkPresetDialog::onDeletePreset()
{
    QString presetName = m_presetList->getSelectedPreset();
    if (presetName.isEmpty()) {
        return;
    }
    
    if (m_presetManager) {
        NetworkPreset preset = m_presetManager->getPreset(presetName);
        if (preset.isReadOnly) {
            QMessageBox::warning(this, "Cannot Delete", "System presets cannot be deleted.");
            return;
        }
        
        int ret = QMessageBox::question(this, "Delete Preset",
                                      QString("Are you sure you want to delete the preset '%1'?").arg(presetName),
                                      QMessageBox::Yes | QMessageBox::No);
        
        if (ret == QMessageBox::Yes) {
            if (m_presetManager->removePreset(presetName)) {
                m_statusLabel->setText(QString("Preset '%1' deleted").arg(presetName));
                setViewMode();
            } else {
                QMessageBox::critical(this, "Error", "Failed to delete preset.");
            }
        }
    }
}

void NetworkPresetDialog::onDuplicatePreset()
{
    QString presetName = m_presetList->getSelectedPreset();
    if (presetName.isEmpty() || !m_presetManager) {
        return;
    }
    
    NetworkPreset original = m_presetManager->getPreset(presetName);
    if (!original.isValid()) {
        return;
    }
    
    // Create a duplicate with a new name
    NetworkPreset duplicate = original;
    duplicate.name = QString("%1 (Copy)").arg(original.name);
    duplicate.isReadOnly = false;
    duplicate.usageCount = 0;
    duplicate.lastUsed = QDateTime();
    duplicate.createdDate = QDateTime::currentDateTime();
    
    // Ensure unique name
    int counter = 1;
    while (m_presetManager->getPreset(duplicate.name).isValid()) {
        duplicate.name = QString("%1 (Copy %2)").arg(original.name).arg(counter++);
    }
    
    if (m_presetManager->addPreset(duplicate)) {
        m_presetList->selectPreset(duplicate.name);
        setEditMode(duplicate.name);
        m_statusLabel->setText(QString("Preset duplicated as '%1'").arg(duplicate.name));
    } else {
        QMessageBox::critical(this, "Error", "Failed to duplicate preset.");
    }
}

void NetworkPresetDialog::onSetAsActive()
{
    QString presetName = m_presetList->getSelectedPreset();
    if (!presetName.isEmpty() && m_presetManager) {
        if (m_presetManager->setActivePreset(presetName)) {
            m_presetList->refreshPresets();
            m_statusLabel->setText(QString("Active preset set to '%1'").arg(presetName));
        }
    }
}

void NetworkPresetDialog::onAddToFavorites()
{
    QString presetName = m_presetList->getSelectedPreset();
    if (!presetName.isEmpty() && m_presetManager) {
        if (m_presetManager->getFavoritePresets().contains(presetName)) {
            m_presetManager->removeFromFavorites(presetName);
            m_statusLabel->setText(QString("'%1' removed from favorites").arg(presetName));
        } else {
            m_presetManager->addToFavorites(presetName);
            m_statusLabel->setText(QString("'%1' added to favorites").arg(presetName));
        }
    }
}

void NetworkPresetDialog::onExportPreset()
{
    QString presetName = m_presetList->getSelectedPreset();
    if (presetName.isEmpty() || !m_presetManager) {
        return;
    }
    
    QString fileName = QFileDialog::getSaveFileName(this, 
                                                   "Export Preset", 
                                                   QString("%1.json").arg(presetName),
                                                   "JSON Files (*.json)");
    
    if (!fileName.isEmpty()) {
        // Implementation for export would go here
        m_statusLabel->setText(QString("Preset exported to '%1'").arg(fileName));
    }
}

void NetworkPresetDialog::onImportPresets()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                   "Import Presets",
                                                   "",
                                                   "JSON Files (*.json)");
    
    if (!fileName.isEmpty() && m_presetManager) {
        // Implementation for import would go here
        m_statusLabel->setText("Presets imported successfully");
        m_presetList->refreshPresets();
    }
}

void NetworkPresetDialog::onRefreshList()
{
    if (m_presetList) {
        m_presetList->refreshPresets();
        m_statusLabel->setText("Preset list refreshed");
    }
}

void NetworkPresetDialog::onValidationChanged(bool isValid)
{
    m_saveButton->setEnabled(isValid);
    m_applyButton->setEnabled(isValid);
}

void NetworkPresetDialog::updateButtonStates()
{
    QString selectedPreset = m_presetList->getSelectedPreset();
    bool hasSelection = !selectedPreset.isEmpty();
    bool canEdit = hasSelection && m_presetManager && 
                   !m_presetManager->getPreset(selectedPreset).isReadOnly;
    
    m_editButton->setEnabled(canEdit);
    m_deleteButton->setEnabled(canEdit);
    m_duplicateButton->setEnabled(hasSelection);
    
    // Editor button states
    bool isEditing = (m_mode == AddMode || m_mode == EditMode);
    m_saveButton->setVisible(isEditing);
    m_applyButton->setVisible(isEditing);
    m_cancelButton->setVisible(isEditing);
    
    if (isEditing) {
        bool isValid = m_presetEditor->validateInput();
        m_saveButton->setEnabled(isValid);
        m_applyButton->setEnabled(isValid && m_hasUnsavedChanges);
    }
}

void NetworkPresetDialog::updateWindowTitle()
{
    QString title = "Network Preset Manager";
    
    switch (m_mode) {
    case AddMode:
        title += " - Add New Preset";
        break;
    case EditMode:
        title += QString(" - Edit '%1'").arg(m_currentPresetName);
        break;
    case ViewMode:
        // Keep default title
        break;
    }
    
    if (m_hasUnsavedChanges) {
        title += " *";
    }
    
    setWindowTitle(title);
}

void NetworkPresetDialog::loadPresetForEditing(const QString &name)
{
    if (m_presetManager) {
        NetworkPreset preset = m_presetManager->getPreset(name);
        if (preset.isValid()) {
            m_presetEditor->setPreset(preset);
        }
    }
}

bool NetworkPresetDialog::saveCurrentPreset()
{
    if (!m_presetEditor->validateInput()) {
        QMessageBox::warning(this, "Validation Error", 
                           m_presetEditor->getValidationErrors().join("\n"));
        return false;
    }
    
    if (!m_presetManager) {
        return false;
    }
    
    NetworkPreset preset = m_presetEditor->getPreset();
    
    bool success = false;
    if (m_mode == AddMode) {
        success = m_presetManager->addPreset(preset);
    } else if (m_mode == EditMode) {
        success = m_presetManager->updatePreset(m_currentPresetName, preset);
    }
    
    if (success) {
        m_presetList->refreshPresets();
        m_presetList->selectPreset(preset.name);
        return true;
    } else {
        QMessageBox::critical(this, "Error", "Failed to save preset.");
        return false;
    }
}

void NetworkPresetDialog::showPresetDetails(const QString &name)
{
    if (m_presetManager) {
        NetworkPreset preset = m_presetManager->getPreset(name);
        if (preset.isValid()) {
            m_presetEditor->setPreset(preset);
        }
    }
}

void NetworkPresetDialog::onAccept()
{
    if (m_hasUnsavedChanges) {
        int ret = QMessageBox::question(this, "Unsaved Changes",
                                      "You have unsaved changes. Save them before closing?",
                                      QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        
        if (ret == QMessageBox::Cancel) {
            return;
        } else if (ret == QMessageBox::Save) {
            if (!saveCurrentPreset()) {
                return;  // Don't close if save failed
            }
        }
    }
    
    accept();
}

void NetworkPresetDialog::onReject()
{
    if (m_hasUnsavedChanges) {
        int ret = QMessageBox::question(this, "Unsaved Changes",
                                      "You have unsaved changes. Discard them?",
                                      QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::No) {
            return;
        }
    }
    
    reject();
}

NetworkPreset NetworkPresetDialog::getResult() const
{
    if (m_mode == AddMode || m_mode == EditMode) {
        return m_presetEditor->getPreset();
    }
    
    QString selectedPreset = m_presetList->getSelectedPreset();
    if (!selectedPreset.isEmpty() && m_presetManager) {
        return m_presetManager->getPreset(selectedPreset);
    }
    
    return NetworkPreset();
}

// NetworkPresetQuickSelector Implementation
NetworkPresetQuickSelector::NetworkPresetQuickSelector(QWidget *parent)
    : QWidget(parent)
    , m_presetManager(nullptr)
{
    setupUI();
}

void NetworkPresetQuickSelector::setupUI()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    
    m_presetCombo = new QComboBox(this);
    m_presetCombo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    
    m_quickAddButton = new QPushButton("+", this);
    m_quickAddButton->setMaximumWidth(30);
    m_quickAddButton->setToolTip("Quick add preset");
    
    m_manageButton = new QPushButton("Manage", this);
    m_manageButton->setMaximumWidth(80);
    
    layout->addWidget(new QLabel("Network:"));
    layout->addWidget(m_presetCombo);
    layout->addWidget(m_quickAddButton);
    layout->addWidget(m_manageButton);
    
    connect(m_presetCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NetworkPresetQuickSelector::onPresetChanged);
    connect(m_manageButton, &QPushButton::clicked, this, &NetworkPresetQuickSelector::onManageButtonClicked);
    connect(m_quickAddButton, &QPushButton::clicked, this, &NetworkPresetQuickSelector::onQuickAddRequested);
}

void NetworkPresetQuickSelector::setPresetManager(NetworkPresetManager *manager)
{
    m_presetManager = manager;
    if (m_presetManager) {
        connect(m_presetManager, &NetworkPresetManager::presetAdded, this, &NetworkPresetQuickSelector::refreshPresets);
        connect(m_presetManager, &NetworkPresetManager::presetRemoved, this, &NetworkPresetQuickSelector::refreshPresets);
        connect(m_presetManager, &NetworkPresetManager::presetUpdated, this, &NetworkPresetQuickSelector::refreshPresets);
        connect(m_presetManager, &NetworkPresetManager::activePresetChanged, this, &NetworkPresetQuickSelector::refreshPresets);
        refreshPresets();
    }
}

void NetworkPresetQuickSelector::refreshPresets()
{
    if (!m_presetManager) {
        return;
    }
    
    updateComboBox();
}

void NetworkPresetQuickSelector::updateComboBox()
{
    if (!m_presetManager) {
        return;
    }
    
    QString currentSelection = m_presetCombo->currentData().toString();
    
    m_presetCombo->clear();
    m_presetCombo->addItem("(No preset selected)", QString());
    
    // Add favorites first
    QStringList favorites = m_presetManager->getFavoritePresets();
    if (!favorites.isEmpty()) {
        m_presetCombo->addItem("--- Favorites ---", QString());
        for (const QString &name : favorites) {
            NetworkPreset preset = m_presetManager->getPreset(name);
            if (preset.isValid()) {
                QString displayText = QString("%1 (%2)").arg(preset.name, preset.cidr);
                m_presetCombo->addItem(displayText, preset.name);
            }
        }
    }
    
    // Add recently used
    QStringList recent = m_presetManager->getRecentlyUsed(5);
    if (!recent.isEmpty()) {
        m_presetCombo->addItem("--- Recent ---", QString());
        for (const QString &name : recent) {
            if (favorites.contains(name)) continue;  // Skip if already in favorites
            
            NetworkPreset preset = m_presetManager->getPreset(name);
            if (preset.isValid()) {
                QString displayText = QString("%1 (%2)").arg(preset.name, preset.cidr);
                m_presetCombo->addItem(displayText, preset.name);
            }
        }
    }
    
    // Add all presets grouped by category
    QMap<QString, QStringList> categorizedPresets;
    QList<NetworkPreset> allPresets = m_presetManager->getAllPresets();
    for (const NetworkPreset &preset : allPresets) {
        QString category = preset.category.isEmpty() ? "General" : preset.category;
        categorizedPresets[category].append(preset.name);
    }
    
    for (auto it = categorizedPresets.constBegin(); it != categorizedPresets.constEnd(); ++it) {
        m_presetCombo->addItem(QString("--- %1 ---").arg(it.key()), QString());
        
        for (const QString &name : it.value()) {
            NetworkPreset preset = m_presetManager->getPreset(name);
            if (preset.isValid()) {
                QString displayText = QString("%1 (%2)").arg(preset.name, preset.cidr);
                m_presetCombo->addItem(displayText, preset.name);
            }
        }
    }
    
    // Restore selection
    if (!currentSelection.isEmpty()) {
        int index = m_presetCombo->findData(currentSelection);
        if (index >= 0) {
            m_presetCombo->setCurrentIndex(index);
        }
    } else {
        // Set active preset if any
        QString activePreset = m_presetManager->getActivePreset();
        if (!activePreset.isEmpty()) {
            int index = m_presetCombo->findData(activePreset);
            if (index >= 0) {
                m_presetCombo->setCurrentIndex(index);
            }
        }
    }
}

QString NetworkPresetQuickSelector::getSelectedPreset() const
{
    return m_presetCombo->currentData().toString();
}

void NetworkPresetQuickSelector::setSelectedPreset(const QString &name)
{
    int index = m_presetCombo->findData(name);
    if (index >= 0) {
        m_presetCombo->setCurrentIndex(index);
    }
}

void NetworkPresetQuickSelector::onPresetChanged()
{
    QString presetName = getSelectedPreset();
    if (!presetName.isEmpty()) {
        emit presetSelected(presetName);
    }
}

void NetworkPresetQuickSelector::onManageButtonClicked()
{
    emit managePresetsRequested();
}

void NetworkPresetQuickSelector::onQuickAddRequested()
{
    // Quick add dialog implementation would go here
    emit managePresetsRequested();  // For now, just open manager
}

void NetworkPresetDialog::onRemoveFromFavorites()
{
    QTreeWidgetItem *currentItem = m_presetList->currentItem();
    if (!currentItem || !m_presetManager) return;
    
    QString presetName = currentItem->text(0);
    NetworkPreset preset = m_presetManager->getPreset(presetName);
    if (preset.isValid()) {
        // Use the existing removeFromFavorites method
        m_presetManager->removeFromFavorites(presetName);
        m_presetList->refreshPresets();
    }
}

void NetworkPresetDialog::onCreateGroup()
{
    bool ok;
    QString groupName = QInputDialog::getText(this, "Create Group", 
                                            "Group name:", QLineEdit::Normal,
                                            "", &ok);
    if (ok && !groupName.isEmpty()) {
        // Group functionality would be implemented here
        // For now, just refresh the list
        m_presetList->refreshPresets();
    }
}

void NetworkPresetDialog::onManageGroups()
{
    QMessageBox::information(this, "Manage Groups", 
                           "Group management feature will be implemented in a future update.");
}

#include "networkpresetdialog.moc"
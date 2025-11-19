#include "mainwindow.h"
#include "presetmanager.h"
#include "activityindicator.h"
#include <QApplication>
#include <QMenuBar>
#include <QToolBar>
#include <QStatusBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include <QGroupBox>
#include <QLabel>
#include <QPushButton>
#include <QTreeWidget>
#include <QTableWidget>
#include <QProgressBar>
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), presetManager(new PresetManager(this)), activityIndicator(new ActivityIndicator(this))
{
    setWindowTitle("FileDuper - Theme Enhanced");
    setMinimumSize(1200, 800);

    setupUI();
    setupConnections();
    setupMenusAndToolbars();
}

MainWindow::~MainWindow() {}

void MainWindow::setupUI()
{
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    QSplitter *splitter = new QSplitter(Qt::Horizontal, this);
    mainLayout->addWidget(splitter);

    // Left panel
    QWidget *leftPanel = new QWidget();
    QVBoxLayout *leftLayout = new QVBoxLayout(leftPanel);

    QGroupBox *themeGroup = new QGroupBox("Themes");
    QVBoxLayout *themeLayout = new QVBoxLayout(themeGroup);

    themeCombo = new QComboBox();
    themeCombo->addItems({"Light", "Dark", "Solarized", "High Contrast"});
    themeLayout->addWidget(themeCombo);

    QPushButton *applyThemeBtn = new QPushButton("Apply Theme");
    themeLayout->addWidget(applyThemeBtn);

    leftLayout->addWidget(themeGroup);

    splitter->addWidget(leftPanel);

    // Right panel
    QWidget *rightPanel = new QWidget();
    QVBoxLayout *rightLayout = new QVBoxLayout(rightPanel);

    QGroupBox *activityGroup = new QGroupBox("Activity");
    QVBoxLayout *activityLayout = new QVBoxLayout(activityGroup);

    activityLayout->addWidget(activityIndicator);

    rightLayout->addWidget(activityGroup);

    splitter->addWidget(rightPanel);
}

void MainWindow::setupConnections()
{
    connect(themeCombo, &QComboBox::currentTextChanged, this, &MainWindow::applyTheme);
}

void MainWindow::setupMenusAndToolbars()
{
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    QMenu *fileMenu = menuBar->addMenu("File");
    QAction *exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
}

void MainWindow::applyTheme(const QString &theme)
{
    if (theme == "Light")
    {
        qApp->setStyleSheet("");
    }
    else if (theme == "Dark")
    {
        qApp->setStyleSheet("QWidget { background-color: #2b2b2b; color: #ffffff; }");
    }
    else if (theme == "Solarized")
    {
        qApp->setStyleSheet("QWidget { background-color: #fdf6e3; color: #657b83; }");
    }
    else if (theme == "High Contrast")
    {
        qApp->setStyleSheet("QWidget { background-color: #000000; color: #ffffff; }");
    }
}

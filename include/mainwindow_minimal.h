#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "scanner.h"

// Forward declarations
class Scanner;
class HashEngine;
class PresetManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onScanProgress(int percentage, int current, int total);
    void onScanCompleted(const DuplicateGroups &results);
    void onHardwareUsageUpdate(const QString &processingUnit, int utilizationPercent, const QString &currentTask);

private:
    Scanner *m_scanner;
    HashEngine *m_hashEngine;
    PresetManager *m_presetManager;
};

#endif // MAINWINDOW_H
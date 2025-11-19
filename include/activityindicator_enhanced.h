#ifndef ACTIVITYINDICATOR_H
#define ACTIVITYINDICATOR_H

#include <QWidget>
#include <QLabel>
#include <QTimer>
#include <QSet>

class ActivityIndicator : public QWidget
{
    Q_OBJECT

public:
    explicit ActivityIndicator(QWidget *parent = nullptr);
    
    void setGpuActive(bool active);
    void setNpuActive(bool active);

private slots:
    void updateHardwareLoads();

private:
    void setupUI();
    void updateLoadDisplay(QLabel *label, int load, const QString &type);
    int getCurrentCpuLoad();
    void startBlinking(QLabel *label);
    void stopBlinking(QLabel *label);
    
    QLabel *cpuLoadLabel;
    QLabel *gpuLoadLabel;
    QLabel *npuLoadLabel;
    QTimer *updateTimer;
    
    bool currentGpuActive = false;
    bool currentNpuActive = false;
    QSet<QLabel*> blinkingLabels;
};

#endif // ACTIVITYINDICATOR_H

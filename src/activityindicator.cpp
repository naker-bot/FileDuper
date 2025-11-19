// ActivityIndicator stub - needs full implementation
#include "activityindicator.h"
#include <QDebug>

ActivityIndicator::ActivityIndicator(QWidget *parent) : QWidget(parent) {
    setupUI();
    qDebug() << "🎯 ActivityIndicator created";
}

ActivityIndicator::~ActivityIndicator() {
    qDebug() << "🎯 ActivityIndicator destructor";
}

void ActivityIndicator::setupUI() {
    layout = new QHBoxLayout(this);
    cpuLoadLabel = new QLabel("CPU: 0%", this);
    gpuLoadLabel = new QLabel("GPU: 0%", this);
    npuLoadLabel = new QLabel("NPU: 0%", this);
    
    layout->addWidget(cpuLoadLabel);
    layout->addWidget(gpuLoadLabel);
    layout->addWidget(npuLoadLabel);
}

void ActivityIndicator::setActive(bool active) { isActive = active; }
void ActivityIndicator::updateLoadDisplay() { /* stub */ }
void ActivityIndicator::blinkCpu() { /* stub */ }
void ActivityIndicator::blinkGpu() { /* stub */ }
void ActivityIndicator::blinkNpu() { /* stub */ }
int ActivityIndicator::getCurrentCpuLoad() { return 0; }
int ActivityIndicator::getCurrentGpuLoad() { return 0; }
int ActivityIndicator::getCurrentNpuLoad() { return 0; }
void ActivityIndicator::updateHardwareDisplay() { /* stub */ }
void ActivityIndicator::updateMemoryDisplay() { /* stub */ }

#include "activityindicator.moc"


void ActivityIndicator::setActivity(bool active, int cpuLoad, int gpuLoad, int npuLoad) {
    isActive = active;
    if (active) {
        if (cpuLoadLabel) cpuLoadLabel->setText(QString("CPU: %1%").arg(cpuLoad));
        if (gpuLoadLabel) gpuLoadLabel->setText(QString("GPU: %1%").arg(gpuLoad));
        if (npuLoadLabel) npuLoadLabel->setText(QString("NPU: %1%").arg(npuLoad));
    }
}

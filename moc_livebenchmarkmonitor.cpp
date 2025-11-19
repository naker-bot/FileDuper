/****************************************************************************
** Meta object code from reading C++ file 'livebenchmarkmonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/livebenchmarkmonitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'livebenchmarkmonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LiveBenchmarkMonitor_t {
    QByteArrayData data[35];
    char stringdata0[436];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LiveBenchmarkMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LiveBenchmarkMonitor_t qt_meta_stringdata_LiveBenchmarkMonitor = {
    {
QT_MOC_LITERAL(0, 0, 20), // "LiveBenchmarkMonitor"
QT_MOC_LITERAL(1, 21, 16), // "onFilesCollected"
QT_MOC_LITERAL(2, 38, 0), // ""
QT_MOC_LITERAL(3, 39, 10), // "totalFiles"
QT_MOC_LITERAL(4, 50, 16), // "onHashingStarted"
QT_MOC_LITERAL(5, 67, 11), // "filesToHash"
QT_MOC_LITERAL(6, 79, 14), // "onScanProgress"
QT_MOC_LITERAL(7, 94, 10), // "percentage"
QT_MOC_LITERAL(8, 105, 7), // "current"
QT_MOC_LITERAL(9, 113, 5), // "total"
QT_MOC_LITERAL(10, 119, 23), // "onCurrentFileProcessing"
QT_MOC_LITERAL(11, 143, 8), // "fileName"
QT_MOC_LITERAL(12, 152, 11), // "processType"
QT_MOC_LITERAL(13, 164, 10), // "fileNumber"
QT_MOC_LITERAL(14, 175, 23), // "onProcessActivityUpdate"
QT_MOC_LITERAL(15, 199, 8), // "activity"
QT_MOC_LITERAL(16, 208, 7), // "details"
QT_MOC_LITERAL(17, 216, 21), // "onHardwareUsageUpdate"
QT_MOC_LITERAL(18, 238, 14), // "processingUnit"
QT_MOC_LITERAL(19, 253, 11), // "utilization"
QT_MOC_LITERAL(20, 265, 4), // "task"
QT_MOC_LITERAL(21, 270, 16), // "onHashCalculated"
QT_MOC_LITERAL(22, 287, 8), // "filePath"
QT_MOC_LITERAL(23, 296, 4), // "hash"
QT_MOC_LITERAL(24, 301, 18), // "onBenchmarkStarted"
QT_MOC_LITERAL(25, 320, 19), // "onBenchmarkFinished"
QT_MOC_LITERAL(26, 340, 8), // "duration"
QT_MOC_LITERAL(27, 349, 12), // "filesScanned"
QT_MOC_LITERAL(28, 362, 5), // "speed"
QT_MOC_LITERAL(29, 368, 16), // "onActivityLogged"
QT_MOC_LITERAL(30, 385, 15), // "onStatusChanged"
QT_MOC_LITERAL(31, 401, 6), // "status"
QT_MOC_LITERAL(32, 408, 8), // "clearLog"
QT_MOC_LITERAL(33, 417, 9), // "exportLog"
QT_MOC_LITERAL(34, 427, 8) // "filename"

    },
    "LiveBenchmarkMonitor\0onFilesCollected\0"
    "\0totalFiles\0onHashingStarted\0filesToHash\0"
    "onScanProgress\0percentage\0current\0"
    "total\0onCurrentFileProcessing\0fileName\0"
    "processType\0fileNumber\0onProcessActivityUpdate\0"
    "activity\0details\0onHardwareUsageUpdate\0"
    "processingUnit\0utilization\0task\0"
    "onHashCalculated\0filePath\0hash\0"
    "onBenchmarkStarted\0onBenchmarkFinished\0"
    "duration\0filesScanned\0speed\0"
    "onActivityLogged\0onStatusChanged\0"
    "status\0clearLog\0exportLog\0filename"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LiveBenchmarkMonitor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x0a /* Public */,
       4,    1,   82,    2, 0x0a /* Public */,
       6,    3,   85,    2, 0x0a /* Public */,
      10,    4,   92,    2, 0x0a /* Public */,
      14,    2,  101,    2, 0x0a /* Public */,
      17,    3,  106,    2, 0x0a /* Public */,
      21,    2,  113,    2, 0x0a /* Public */,
      24,    0,  118,    2, 0x0a /* Public */,
      25,    3,  119,    2, 0x0a /* Public */,
      29,    2,  126,    2, 0x0a /* Public */,
      30,    1,  131,    2, 0x0a /* Public */,
      32,    0,  134,    2, 0x0a /* Public */,
      33,    1,  135,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Int,    7,    8,    9,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Int, QMetaType::Int,   11,   12,   13,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString,   18,   19,   20,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   22,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Double, QMetaType::Int, QMetaType::Double,   26,   27,   28,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   15,   16,
    QMetaType::Void, QMetaType::QString,   31,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   34,

       0        // eod
};

void LiveBenchmarkMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LiveBenchmarkMonitor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onFilesCollected((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->onHashingStarted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->onScanProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 3: _t->onCurrentFileProcessing((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3])),(*reinterpret_cast< int(*)>(_a[4]))); break;
        case 4: _t->onProcessActivityUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 5: _t->onHardwareUsageUpdate((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 6: _t->onHashCalculated((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 7: _t->onBenchmarkStarted(); break;
        case 8: _t->onBenchmarkFinished((*reinterpret_cast< double(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 9: _t->onActivityLogged((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->onStatusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->clearLog(); break;
        case 12: _t->exportLog((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LiveBenchmarkMonitor::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_LiveBenchmarkMonitor.data,
    qt_meta_data_LiveBenchmarkMonitor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LiveBenchmarkMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LiveBenchmarkMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LiveBenchmarkMonitor.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LiveBenchmarkMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

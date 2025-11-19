/****************************************************************************
** Meta object code from reading C++ file 'hardwaremonitor.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/hardwaremonitor.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hardwaremonitor.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HardwareMonitor_t {
    QByteArrayData data[9];
    char stringdata0[129];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HardwareMonitor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HardwareMonitor_t qt_meta_stringdata_HardwareMonitor = {
    {
QT_MOC_LITERAL(0, 0, 15), // "HardwareMonitor"
QT_MOC_LITERAL(1, 16, 19), // "updateHardwareStats"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 13), // "onScanStarted"
QT_MOC_LITERAL(4, 51, 14), // "onScanFinished"
QT_MOC_LITERAL(5, 66, 14), // "updateCpuUsage"
QT_MOC_LITERAL(6, 81, 14), // "updateGpuUsage"
QT_MOC_LITERAL(7, 96, 14), // "updateNpuUsage"
QT_MOC_LITERAL(8, 111, 17) // "updateMemoryUsage"

    },
    "HardwareMonitor\0updateHardwareStats\0"
    "\0onScanStarted\0onScanFinished\0"
    "updateCpuUsage\0updateGpuUsage\0"
    "updateNpuUsage\0updateMemoryUsage"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HardwareMonitor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x0a /* Public */,
       3,    0,   50,    2, 0x0a /* Public */,
       4,    0,   51,    2, 0x0a /* Public */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    0,   54,    2, 0x08 /* Private */,
       8,    0,   55,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void HardwareMonitor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HardwareMonitor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->updateHardwareStats(); break;
        case 1: _t->onScanStarted(); break;
        case 2: _t->onScanFinished(); break;
        case 3: _t->updateCpuUsage(); break;
        case 4: _t->updateGpuUsage(); break;
        case 5: _t->updateNpuUsage(); break;
        case 6: _t->updateMemoryUsage(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject HardwareMonitor::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_HardwareMonitor.data,
    qt_meta_data_HardwareMonitor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HardwareMonitor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HardwareMonitor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HardwareMonitor.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HardwareMonitor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 7)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 7;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 7)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 7;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

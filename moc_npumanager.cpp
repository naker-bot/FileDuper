/****************************************************************************
** Meta object code from reading C++ file 'npumanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/npumanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'npumanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NpuManager_t {
    QByteArrayData data[33];
    char stringdata0[473];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NpuManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NpuManager_t qt_meta_stringdata_NpuManager = {
    {
QT_MOC_LITERAL(0, 0, 10), // "NpuManager"
QT_MOC_LITERAL(1, 11, 16), // "hardwareDetected"
QT_MOC_LITERAL(2, 28, 0), // ""
QT_MOC_LITERAL(3, 29, 18), // "utilizationChanged"
QT_MOC_LITERAL(4, 48, 7), // "npuLoad"
QT_MOC_LITERAL(5, 56, 7), // "gpuLoad"
QT_MOC_LITERAL(6, 64, 17), // "capabilityChanged"
QT_MOC_LITERAL(7, 82, 13), // "NpuCapability"
QT_MOC_LITERAL(8, 96, 10), // "capability"
QT_MOC_LITERAL(9, 107, 9), // "available"
QT_MOC_LITERAL(10, 117, 5), // "error"
QT_MOC_LITERAL(11, 123, 7), // "message"
QT_MOC_LITERAL(12, 131, 14), // "npuLoadChanged"
QT_MOC_LITERAL(13, 146, 4), // "load"
QT_MOC_LITERAL(14, 151, 14), // "gpuLoadChanged"
QT_MOC_LITERAL(15, 166, 19), // "imageBatchProcessed"
QT_MOC_LITERAL(16, 186, 15), // "processedImages"
QT_MOC_LITERAL(17, 202, 21), // "imageFeatureExtracted"
QT_MOC_LITERAL(18, 224, 12), // "ImageFeature"
QT_MOC_LITERAL(19, 237, 7), // "feature"
QT_MOC_LITERAL(20, 245, 19), // "duplicateGroupFound"
QT_MOC_LITERAL(21, 265, 17), // "NpuDuplicateGroup"
QT_MOC_LITERAL(22, 283, 5), // "group"
QT_MOC_LITERAL(23, 289, 25), // "featureExtractionProgress"
QT_MOC_LITERAL(24, 315, 9), // "processed"
QT_MOC_LITERAL(25, 325, 5), // "total"
QT_MOC_LITERAL(26, 331, 24), // "featureExtractionMessage"
QT_MOC_LITERAL(27, 356, 27), // "similarityAnalysisCompleted"
QT_MOC_LITERAL(28, 384, 24), // "QList<NpuDuplicateGroup>"
QT_MOC_LITERAL(29, 409, 6), // "groups"
QT_MOC_LITERAL(30, 416, 14), // "detectHardware"
QT_MOC_LITERAL(31, 431, 17), // "updateUtilization"
QT_MOC_LITERAL(32, 449, 23) // "onHardwareCheckFinished"

    },
    "NpuManager\0hardwareDetected\0\0"
    "utilizationChanged\0npuLoad\0gpuLoad\0"
    "capabilityChanged\0NpuCapability\0"
    "capability\0available\0error\0message\0"
    "npuLoadChanged\0load\0gpuLoadChanged\0"
    "imageBatchProcessed\0processedImages\0"
    "imageFeatureExtracted\0ImageFeature\0"
    "feature\0duplicateGroupFound\0"
    "NpuDuplicateGroup\0group\0"
    "featureExtractionProgress\0processed\0"
    "total\0featureExtractionMessage\0"
    "similarityAnalysisCompleted\0"
    "QList<NpuDuplicateGroup>\0groups\0"
    "detectHardware\0updateUtilization\0"
    "onHardwareCheckFinished"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NpuManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      15,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      12,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   89,    2, 0x06 /* Public */,
       3,    2,   90,    2, 0x06 /* Public */,
       6,    2,   95,    2, 0x06 /* Public */,
      10,    1,  100,    2, 0x06 /* Public */,
      12,    1,  103,    2, 0x06 /* Public */,
      14,    1,  106,    2, 0x06 /* Public */,
      15,    1,  109,    2, 0x06 /* Public */,
      17,    1,  112,    2, 0x06 /* Public */,
      20,    1,  115,    2, 0x06 /* Public */,
      23,    2,  118,    2, 0x06 /* Public */,
      26,    1,  123,    2, 0x06 /* Public */,
      27,    1,  126,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      30,    0,  129,    2, 0x0a /* Public */,
      31,    0,  130,    2, 0x0a /* Public */,
      32,    0,  131,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,    4,    5,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Bool,    8,    9,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::Int,   13,
    QMetaType::Void, QMetaType::QStringList,   16,
    QMetaType::Void, 0x80000000 | 18,   19,
    QMetaType::Void, 0x80000000 | 21,   22,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   24,   25,
    QMetaType::Void, QMetaType::QString,   11,
    QMetaType::Void, 0x80000000 | 28,   29,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NpuManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NpuManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->hardwareDetected(); break;
        case 1: _t->utilizationChanged((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 2: _t->capabilityChanged((*reinterpret_cast< NpuCapability(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 3: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->npuLoadChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 5: _t->gpuLoadChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->imageBatchProcessed((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 7: _t->imageFeatureExtracted((*reinterpret_cast< const ImageFeature(*)>(_a[1]))); break;
        case 8: _t->duplicateGroupFound((*reinterpret_cast< const NpuDuplicateGroup(*)>(_a[1]))); break;
        case 9: _t->featureExtractionProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 10: _t->featureExtractionMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->similarityAnalysisCompleted((*reinterpret_cast< const QList<NpuDuplicateGroup>(*)>(_a[1]))); break;
        case 12: _t->detectHardware(); break;
        case 13: _t->updateUtilization(); break;
        case 14: _t->onHardwareCheckFinished(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NpuManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::hardwareDetected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::utilizationChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(NpuCapability , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::capabilityChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::error)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::npuLoadChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::gpuLoadChanged)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::imageBatchProcessed)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(const ImageFeature & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::imageFeatureExtracted)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(const NpuDuplicateGroup & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::duplicateGroupFound)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::featureExtractionProgress)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::featureExtractionMessage)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (NpuManager::*)(const QList<NpuDuplicateGroup> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NpuManager::similarityAnalysisCompleted)) {
                *result = 11;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NpuManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NpuManager.data,
    qt_meta_data_NpuManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NpuManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NpuManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NpuManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NpuManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void NpuManager::hardwareDetected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NpuManager::utilizationChanged(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NpuManager::capabilityChanged(NpuCapability _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NpuManager::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NpuManager::npuLoadChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NpuManager::gpuLoadChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NpuManager::imageBatchProcessed(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NpuManager::imageFeatureExtracted(const ImageFeature & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void NpuManager::duplicateGroupFound(const NpuDuplicateGroup & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void NpuManager::featureExtractionProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void NpuManager::featureExtractionMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void NpuManager::similarityAnalysisCompleted(const QList<NpuDuplicateGroup> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

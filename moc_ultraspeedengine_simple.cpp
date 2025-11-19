/****************************************************************************
** Meta object code from reading C++ file 'ultraspeedengine_simple.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/ultraspeedengine_simple.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ultraspeedengine_simple.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_UltraSpeedEngine_t {
    QByteArrayData data[26];
    char stringdata0[349];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_UltraSpeedEngine_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_UltraSpeedEngine_t qt_meta_stringdata_UltraSpeedEngine = {
    {
QT_MOC_LITERAL(0, 0, 16), // "UltraSpeedEngine"
QT_MOC_LITERAL(1, 17, 11), // "scanStarted"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 12), // "scanProgress"
QT_MOC_LITERAL(4, 43, 9), // "processed"
QT_MOC_LITERAL(5, 53, 5), // "total"
QT_MOC_LITERAL(6, 59, 10), // "percentage"
QT_MOC_LITERAL(7, 70, 13), // "scanCompleted"
QT_MOC_LITERAL(8, 84, 26), // "QHash<QString,QStringList>"
QT_MOC_LITERAL(9, 111, 15), // "duplicateGroups"
QT_MOC_LITERAL(10, 127, 21), // "QHash<QString,qint64>"
QT_MOC_LITERAL(11, 149, 9), // "fileSizes"
QT_MOC_LITERAL(12, 159, 9), // "scanError"
QT_MOC_LITERAL(13, 169, 5), // "error"
QT_MOC_LITERAL(14, 175, 18), // "currentFileChanged"
QT_MOC_LITERAL(15, 194, 8), // "filePath"
QT_MOC_LITERAL(16, 203, 13), // "imageProgress"
QT_MOC_LITERAL(17, 217, 16), // "npuStatusChanged"
QT_MOC_LITERAL(18, 234, 6), // "status"
QT_MOC_LITERAL(19, 241, 27), // "featureBasedDuplicatesFound"
QT_MOC_LITERAL(20, 269, 5), // "count"
QT_MOC_LITERAL(21, 275, 22), // "imageProcessingMessage"
QT_MOC_LITERAL(22, 298, 7), // "message"
QT_MOC_LITERAL(23, 306, 12), // "processFiles"
QT_MOC_LITERAL(24, 319, 11), // "updateStats"
QT_MOC_LITERAL(25, 331, 17) // "processImageFiles"

    },
    "UltraSpeedEngine\0scanStarted\0\0"
    "scanProgress\0processed\0total\0percentage\0"
    "scanCompleted\0QHash<QString,QStringList>\0"
    "duplicateGroups\0QHash<QString,qint64>\0"
    "fileSizes\0scanError\0error\0currentFileChanged\0"
    "filePath\0imageProgress\0npuStatusChanged\0"
    "status\0featureBasedDuplicatesFound\0"
    "count\0imageProcessingMessage\0message\0"
    "processFiles\0updateStats\0processImageFiles"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_UltraSpeedEngine[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    3,   75,    2, 0x06 /* Public */,
       7,    2,   82,    2, 0x06 /* Public */,
      12,    1,   87,    2, 0x06 /* Public */,
      14,    1,   90,    2, 0x06 /* Public */,
      16,    3,   93,    2, 0x06 /* Public */,
      17,    1,  100,    2, 0x06 /* Public */,
      19,    1,  103,    2, 0x06 /* Public */,
      21,    1,  106,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      23,    0,  109,    2, 0x08 /* Private */,
      24,    0,  110,    2, 0x08 /* Private */,
      25,    0,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double,    4,    5,    6,
    QMetaType::Void, 0x80000000 | 8, 0x80000000 | 10,    9,   11,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::QString,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::Double,    4,    5,    6,
    QMetaType::Void, QMetaType::QString,   18,
    QMetaType::Void, QMetaType::Int,   20,
    QMetaType::Void, QMetaType::QString,   22,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void UltraSpeedEngine::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<UltraSpeedEngine *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->scanStarted(); break;
        case 1: _t->scanProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 2: _t->scanCompleted((*reinterpret_cast< const QHash<QString,QStringList>(*)>(_a[1])),(*reinterpret_cast< const QHash<QString,qint64>(*)>(_a[2]))); break;
        case 3: _t->scanError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->currentFileChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->imageProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< double(*)>(_a[3]))); break;
        case 6: _t->npuStatusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->featureBasedDuplicatesFound((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->imageProcessingMessage((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->processFiles(); break;
        case 10: _t->updateStats(); break;
        case 11: _t->processImageFiles(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (UltraSpeedEngine::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::scanStarted)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (UltraSpeedEngine::*)(int , int , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::scanProgress)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (UltraSpeedEngine::*)(const QHash<QString,QStringList> & , const QHash<QString,qint64> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::scanCompleted)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (UltraSpeedEngine::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::scanError)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (UltraSpeedEngine::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::currentFileChanged)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (UltraSpeedEngine::*)(int , int , double );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::imageProgress)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (UltraSpeedEngine::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::npuStatusChanged)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (UltraSpeedEngine::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::featureBasedDuplicatesFound)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (UltraSpeedEngine::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&UltraSpeedEngine::imageProcessingMessage)) {
                *result = 8;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject UltraSpeedEngine::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_UltraSpeedEngine.data,
    qt_meta_data_UltraSpeedEngine,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *UltraSpeedEngine::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *UltraSpeedEngine::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_UltraSpeedEngine.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int UltraSpeedEngine::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void UltraSpeedEngine::scanStarted()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void UltraSpeedEngine::scanProgress(int _t1, int _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void UltraSpeedEngine::scanCompleted(const QHash<QString,QStringList> & _t1, const QHash<QString,qint64> & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void UltraSpeedEngine::scanError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void UltraSpeedEngine::currentFileChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void UltraSpeedEngine::imageProgress(int _t1, int _t2, double _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void UltraSpeedEngine::npuStatusChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void UltraSpeedEngine::featureBasedDuplicatesFound(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void UltraSpeedEngine::imageProcessingMessage(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'ftpclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/ftpclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FtpClient_t {
    QByteArrayData data[20];
    char stringdata0[218];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FtpClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FtpClient_t qt_meta_stringdata_FtpClient = {
    {
QT_MOC_LITERAL(0, 0, 9), // "FtpClient"
QT_MOC_LITERAL(1, 10, 9), // "connected"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 12), // "disconnected"
QT_MOC_LITERAL(4, 34, 5), // "error"
QT_MOC_LITERAL(5, 40, 7), // "message"
QT_MOC_LITERAL(6, 48, 24), // "directoryListingReceived"
QT_MOC_LITERAL(7, 73, 5), // "files"
QT_MOC_LITERAL(8, 79, 12), // "listFinished"
QT_MOC_LITERAL(9, 92, 7), // "allDirs"
QT_MOC_LITERAL(10, 100, 7), // "success"
QT_MOC_LITERAL(11, 108, 17), // "filesListFinished"
QT_MOC_LITERAL(12, 126, 9), // "directory"
QT_MOC_LITERAL(13, 136, 19), // "subdirectoriesFound"
QT_MOC_LITERAL(14, 156, 7), // "subdirs"
QT_MOC_LITERAL(15, 164, 14), // "removeFinished"
QT_MOC_LITERAL(16, 179, 4), // "file"
QT_MOC_LITERAL(17, 184, 12), // "scanProgress"
QT_MOC_LITERAL(18, 197, 9), // "iteration"
QT_MOC_LITERAL(19, 207, 10) // "totalFiles"

    },
    "FtpClient\0connected\0\0disconnected\0"
    "error\0message\0directoryListingReceived\0"
    "files\0listFinished\0allDirs\0success\0"
    "filesListFinished\0directory\0"
    "subdirectoriesFound\0subdirs\0removeFinished\0"
    "file\0scanProgress\0iteration\0totalFiles"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FtpClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,
       6,    1,   64,    2, 0x06 /* Public */,
       8,    2,   67,    2, 0x06 /* Public */,
      11,    3,   72,    2, 0x06 /* Public */,
      13,    2,   79,    2, 0x06 /* Public */,
      15,    2,   84,    2, 0x06 /* Public */,
      17,    3,   89,    2, 0x06 /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QStringList,    7,
    QMetaType::Void, QMetaType::QStringList, QMetaType::Bool,    9,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList, QMetaType::Bool,   12,    7,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList,   12,   14,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   16,   10,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,   12,   18,   19,

       0        // eod
};

void FtpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FtpClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->directoryListingReceived((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 4: _t->listFinished((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 5: _t->filesListFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2])),(*reinterpret_cast< bool(*)>(_a[3]))); break;
        case 6: _t->subdirectoriesFound((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 7: _t->removeFinished((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 8: _t->scanProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (FtpClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (FtpClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (FtpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::error)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (FtpClient::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::directoryListingReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (FtpClient::*)(const QStringList & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::listFinished)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (FtpClient::*)(const QString & , const QStringList & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::filesListFinished)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (FtpClient::*)(const QString & , const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::subdirectoriesFound)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (FtpClient::*)(const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::removeFinished)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (FtpClient::*)(const QString & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&FtpClient::scanProgress)) {
                *result = 8;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FtpClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_FtpClient.data,
    qt_meta_data_FtpClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FtpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FtpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FtpClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int FtpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void FtpClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void FtpClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void FtpClient::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void FtpClient::directoryListingReceived(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void FtpClient::listFinished(const QStringList & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void FtpClient::filesListFinished(const QString & _t1, const QStringList & _t2, bool _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void FtpClient::subdirectoriesFound(const QString & _t1, const QStringList & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void FtpClient::removeFinished(const QString & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void FtpClient::scanProgress(const QString & _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

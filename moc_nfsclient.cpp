/****************************************************************************
** Meta object code from reading C++ file 'nfsclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/nfsclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nfsclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NfsClient_t {
    QByteArrayData data[21];
    char stringdata0[286];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NfsClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NfsClient_t qt_meta_stringdata_NfsClient = {
    {
QT_MOC_LITERAL(0, 0, 9), // "NfsClient"
QT_MOC_LITERAL(1, 10, 9), // "connected"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 12), // "disconnected"
QT_MOC_LITERAL(4, 34, 13), // "errorOccurred"
QT_MOC_LITERAL(5, 48, 7), // "message"
QT_MOC_LITERAL(6, 56, 19), // "exportsListReceived"
QT_MOC_LITERAL(7, 76, 20), // "QList<NfsExportInfo>"
QT_MOC_LITERAL(8, 97, 7), // "exports"
QT_MOC_LITERAL(9, 105, 13), // "exportMounted"
QT_MOC_LITERAL(10, 119, 10), // "mountPoint"
QT_MOC_LITERAL(11, 130, 15), // "exportUnmounted"
QT_MOC_LITERAL(12, 146, 24), // "directoryListingReceived"
QT_MOC_LITERAL(13, 171, 5), // "files"
QT_MOC_LITERAL(14, 177, 20), // "onNfsProcessFinished"
QT_MOC_LITERAL(15, 198, 8), // "exitCode"
QT_MOC_LITERAL(16, 207, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(17, 228, 10), // "exitStatus"
QT_MOC_LITERAL(18, 239, 17), // "onNfsProcessError"
QT_MOC_LITERAL(19, 257, 22), // "QProcess::ProcessError"
QT_MOC_LITERAL(20, 280, 5) // "error"

    },
    "NfsClient\0connected\0\0disconnected\0"
    "errorOccurred\0message\0exportsListReceived\0"
    "QList<NfsExportInfo>\0exports\0exportMounted\0"
    "mountPoint\0exportUnmounted\0"
    "directoryListingReceived\0files\0"
    "onNfsProcessFinished\0exitCode\0"
    "QProcess::ExitStatus\0exitStatus\0"
    "onNfsProcessError\0QProcess::ProcessError\0"
    "error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NfsClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   59,    2, 0x06 /* Public */,
       3,    0,   60,    2, 0x06 /* Public */,
       4,    1,   61,    2, 0x06 /* Public */,
       6,    1,   64,    2, 0x06 /* Public */,
       9,    1,   67,    2, 0x06 /* Public */,
      11,    1,   70,    2, 0x06 /* Public */,
      12,    1,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    2,   76,    2, 0x08 /* Private */,
      18,    1,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, 0x80000000 | 7,   13,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 16,   15,   17,
    QMetaType::Void, 0x80000000 | 19,   20,

       0        // eod
};

void NfsClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NfsClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->exportsListReceived((*reinterpret_cast< const QList<NfsExportInfo>(*)>(_a[1]))); break;
        case 4: _t->exportMounted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->exportUnmounted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->directoryListingReceived((*reinterpret_cast< const QList<NfsExportInfo>(*)>(_a[1]))); break;
        case 7: _t->onNfsProcessFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 8: _t->onNfsProcessError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NfsClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NfsClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NfsClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NfsClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NfsClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NfsClient::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NfsClient::*)(const QList<NfsExportInfo> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NfsClient::exportsListReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NfsClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NfsClient::exportMounted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NfsClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NfsClient::exportUnmounted)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NfsClient::*)(const QList<NfsExportInfo> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NfsClient::directoryListingReceived)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NfsClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NfsClient.data,
    qt_meta_data_NfsClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NfsClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NfsClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NfsClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NfsClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void NfsClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NfsClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NfsClient::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NfsClient::exportsListReceived(const QList<NfsExportInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NfsClient::exportMounted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NfsClient::exportUnmounted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NfsClient::directoryListingReceived(const QList<NfsExportInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

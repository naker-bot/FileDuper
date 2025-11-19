/****************************************************************************
** Meta object code from reading C++ file 'smbclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/smbclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'smbclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SmbClient_t {
    QByteArrayData data[22];
    char stringdata0[291];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SmbClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SmbClient_t qt_meta_stringdata_SmbClient = {
    {
QT_MOC_LITERAL(0, 0, 9), // "SmbClient"
QT_MOC_LITERAL(1, 10, 9), // "connected"
QT_MOC_LITERAL(2, 20, 0), // ""
QT_MOC_LITERAL(3, 21, 12), // "disconnected"
QT_MOC_LITERAL(4, 34, 13), // "errorOccurred"
QT_MOC_LITERAL(5, 48, 7), // "message"
QT_MOC_LITERAL(6, 56, 18), // "sharesListReceived"
QT_MOC_LITERAL(7, 75, 19), // "QList<SmbShareInfo>"
QT_MOC_LITERAL(8, 95, 6), // "shares"
QT_MOC_LITERAL(9, 102, 24), // "directoryListingReceived"
QT_MOC_LITERAL(10, 127, 5), // "files"
QT_MOC_LITERAL(11, 133, 14), // "fileDownloaded"
QT_MOC_LITERAL(12, 148, 9), // "localPath"
QT_MOC_LITERAL(13, 158, 12), // "fileUploaded"
QT_MOC_LITERAL(14, 171, 10), // "remotePath"
QT_MOC_LITERAL(15, 182, 20), // "onSmbProcessFinished"
QT_MOC_LITERAL(16, 203, 8), // "exitCode"
QT_MOC_LITERAL(17, 212, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(18, 233, 10), // "exitStatus"
QT_MOC_LITERAL(19, 244, 17), // "onSmbProcessError"
QT_MOC_LITERAL(20, 262, 22), // "QProcess::ProcessError"
QT_MOC_LITERAL(21, 285, 5) // "error"

    },
    "SmbClient\0connected\0\0disconnected\0"
    "errorOccurred\0message\0sharesListReceived\0"
    "QList<SmbShareInfo>\0shares\0"
    "directoryListingReceived\0files\0"
    "fileDownloaded\0localPath\0fileUploaded\0"
    "remotePath\0onSmbProcessFinished\0"
    "exitCode\0QProcess::ExitStatus\0exitStatus\0"
    "onSmbProcessError\0QProcess::ProcessError\0"
    "error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SmbClient[] = {

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
      13,    1,   73,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      15,    2,   76,    2, 0x08 /* Private */,
      19,    1,   81,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,   10,
    QMetaType::Void, QMetaType::QString,   12,
    QMetaType::Void, QMetaType::QString,   14,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 17,   16,   18,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void SmbClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SmbClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->sharesListReceived((*reinterpret_cast< const QList<SmbShareInfo>(*)>(_a[1]))); break;
        case 4: _t->directoryListingReceived((*reinterpret_cast< const QList<SmbShareInfo>(*)>(_a[1]))); break;
        case 5: _t->fileDownloaded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->fileUploaded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->onSmbProcessFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 8: _t->onSmbProcessError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SmbClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmbClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SmbClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmbClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SmbClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmbClient::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SmbClient::*)(const QList<SmbShareInfo> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmbClient::sharesListReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SmbClient::*)(const QList<SmbShareInfo> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmbClient::directoryListingReceived)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (SmbClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmbClient::fileDownloaded)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (SmbClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SmbClient::fileUploaded)) {
                *result = 6;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SmbClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SmbClient.data,
    qt_meta_data_SmbClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SmbClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SmbClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SmbClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SmbClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void SmbClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SmbClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SmbClient::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SmbClient::sharesListReceived(const QList<SmbShareInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SmbClient::directoryListingReceived(const QList<SmbShareInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SmbClient::fileDownloaded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void SmbClient::fileUploaded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

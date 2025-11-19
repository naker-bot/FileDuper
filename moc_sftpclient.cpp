/****************************************************************************
** Meta object code from reading C++ file 'sftpclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/sftpclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#include <QtCore/QList>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sftpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_SftpClient_t {
    QByteArrayData data[20];
    char stringdata0[268];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_SftpClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_SftpClient_t qt_meta_stringdata_SftpClient = {
    {
QT_MOC_LITERAL(0, 0, 10), // "SftpClient"
QT_MOC_LITERAL(1, 11, 9), // "connected"
QT_MOC_LITERAL(2, 21, 0), // ""
QT_MOC_LITERAL(3, 22, 12), // "disconnected"
QT_MOC_LITERAL(4, 35, 13), // "errorOccurred"
QT_MOC_LITERAL(5, 49, 7), // "message"
QT_MOC_LITERAL(6, 57, 24), // "directoryListingReceived"
QT_MOC_LITERAL(7, 82, 19), // "QList<SftpFileInfo>"
QT_MOC_LITERAL(8, 102, 5), // "files"
QT_MOC_LITERAL(9, 108, 14), // "fileDownloaded"
QT_MOC_LITERAL(10, 123, 9), // "localPath"
QT_MOC_LITERAL(11, 133, 12), // "fileUploaded"
QT_MOC_LITERAL(12, 146, 10), // "remotePath"
QT_MOC_LITERAL(13, 157, 21), // "onSftpProcessFinished"
QT_MOC_LITERAL(14, 179, 8), // "exitCode"
QT_MOC_LITERAL(15, 188, 20), // "QProcess::ExitStatus"
QT_MOC_LITERAL(16, 209, 10), // "exitStatus"
QT_MOC_LITERAL(17, 220, 18), // "onSftpProcessError"
QT_MOC_LITERAL(18, 239, 22), // "QProcess::ProcessError"
QT_MOC_LITERAL(19, 262, 5) // "error"

    },
    "SftpClient\0connected\0\0disconnected\0"
    "errorOccurred\0message\0directoryListingReceived\0"
    "QList<SftpFileInfo>\0files\0fileDownloaded\0"
    "localPath\0fileUploaded\0remotePath\0"
    "onSftpProcessFinished\0exitCode\0"
    "QProcess::ExitStatus\0exitStatus\0"
    "onSftpProcessError\0QProcess::ProcessError\0"
    "error"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_SftpClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       6,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   54,    2, 0x06 /* Public */,
       3,    0,   55,    2, 0x06 /* Public */,
       4,    1,   56,    2, 0x06 /* Public */,
       6,    1,   59,    2, 0x06 /* Public */,
       9,    1,   62,    2, 0x06 /* Public */,
      11,    1,   65,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      13,    2,   68,    2, 0x08 /* Private */,
      17,    1,   73,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString,   10,
    QMetaType::Void, QMetaType::QString,   12,

 // slots: parameters
    QMetaType::Void, QMetaType::Int, 0x80000000 | 15,   14,   16,
    QMetaType::Void, 0x80000000 | 18,   19,

       0        // eod
};

void SftpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<SftpClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->directoryListingReceived((*reinterpret_cast< const QList<SftpFileInfo>(*)>(_a[1]))); break;
        case 4: _t->fileDownloaded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->fileUploaded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->onSftpProcessFinished((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< QProcess::ExitStatus(*)>(_a[2]))); break;
        case 7: _t->onSftpProcessError((*reinterpret_cast< QProcess::ProcessError(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (SftpClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SftpClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (SftpClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SftpClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (SftpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SftpClient::errorOccurred)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (SftpClient::*)(const QList<SftpFileInfo> & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SftpClient::directoryListingReceived)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (SftpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SftpClient::fileDownloaded)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (SftpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&SftpClient::fileUploaded)) {
                *result = 5;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject SftpClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_SftpClient.data,
    qt_meta_data_SftpClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *SftpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SftpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_SftpClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SftpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void SftpClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void SftpClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void SftpClient::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void SftpClient::directoryListingReceived(const QList<SftpFileInfo> & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void SftpClient::fileDownloaded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void SftpClient::fileUploaded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

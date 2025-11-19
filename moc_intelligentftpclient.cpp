/****************************************************************************
** Meta object code from reading C++ file 'intelligentftpclient.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/intelligentftpclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'intelligentftpclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_IntelligentFtpClient_t {
    QByteArrayData data[45];
    char stringdata0[638];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_IntelligentFtpClient_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_IntelligentFtpClient_t qt_meta_stringdata_IntelligentFtpClient = {
    {
QT_MOC_LITERAL(0, 0, 20), // "IntelligentFtpClient"
QT_MOC_LITERAL(1, 21, 9), // "connected"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "disconnected"
QT_MOC_LITERAL(4, 45, 15), // "connectionError"
QT_MOC_LITERAL(5, 61, 5), // "error"
QT_MOC_LITERAL(6, 67, 17), // "connectionTimeout"
QT_MOC_LITERAL(7, 85, 11), // "scanStarted"
QT_MOC_LITERAL(8, 97, 4), // "path"
QT_MOC_LITERAL(9, 102, 12), // "scanProgress"
QT_MOC_LITERAL(10, 115, 12), // "filesScanned"
QT_MOC_LITERAL(11, 128, 10), // "totalFiles"
QT_MOC_LITERAL(12, 139, 11), // "currentFile"
QT_MOC_LITERAL(13, 151, 13), // "scanCompleted"
QT_MOC_LITERAL(14, 165, 13), // "FtpScanResult"
QT_MOC_LITERAL(15, 179, 6), // "result"
QT_MOC_LITERAL(16, 186, 16), // "directoryChanged"
QT_MOC_LITERAL(17, 203, 7), // "newPath"
QT_MOC_LITERAL(18, 211, 19), // "fileDownloadStarted"
QT_MOC_LITERAL(19, 231, 10), // "remotePath"
QT_MOC_LITERAL(20, 242, 20), // "fileDownloadProgress"
QT_MOC_LITERAL(21, 263, 4), // "file"
QT_MOC_LITERAL(22, 268, 13), // "bytesReceived"
QT_MOC_LITERAL(23, 282, 10), // "bytesTotal"
QT_MOC_LITERAL(24, 293, 21), // "fileDownloadCompleted"
QT_MOC_LITERAL(25, 315, 9), // "localPath"
QT_MOC_LITERAL(26, 325, 19), // "fileDeleteCompleted"
QT_MOC_LITERAL(27, 345, 7), // "success"
QT_MOC_LITERAL(28, 353, 18), // "fileOperationError"
QT_MOC_LITERAL(29, 372, 9), // "operation"
QT_MOC_LITERAL(30, 382, 21), // "batchOperationStarted"
QT_MOC_LITERAL(31, 404, 22), // "batchOperationProgress"
QT_MOC_LITERAL(32, 427, 14), // "completedFiles"
QT_MOC_LITERAL(33, 442, 23), // "batchOperationCompleted"
QT_MOC_LITERAL(34, 466, 12), // "successCount"
QT_MOC_LITERAL(35, 479, 9), // "failCount"
QT_MOC_LITERAL(36, 489, 13), // "statusChanged"
QT_MOC_LITERAL(37, 503, 6), // "status"
QT_MOC_LITERAL(38, 510, 13), // "errorOccurred"
QT_MOC_LITERAL(39, 524, 22), // "onNetworkReplyFinished"
QT_MOC_LITERAL(40, 547, 14), // "onNetworkError"
QT_MOC_LITERAL(41, 562, 27), // "QNetworkReply::NetworkError"
QT_MOC_LITERAL(42, 590, 16), // "onProgressUpdate"
QT_MOC_LITERAL(43, 607, 16), // "onReconnectTimer"
QT_MOC_LITERAL(44, 624, 13) // "onScanTimeout"

    },
    "IntelligentFtpClient\0connected\0\0"
    "disconnected\0connectionError\0error\0"
    "connectionTimeout\0scanStarted\0path\0"
    "scanProgress\0filesScanned\0totalFiles\0"
    "currentFile\0scanCompleted\0FtpScanResult\0"
    "result\0directoryChanged\0newPath\0"
    "fileDownloadStarted\0remotePath\0"
    "fileDownloadProgress\0file\0bytesReceived\0"
    "bytesTotal\0fileDownloadCompleted\0"
    "localPath\0fileDeleteCompleted\0success\0"
    "fileOperationError\0operation\0"
    "batchOperationStarted\0batchOperationProgress\0"
    "completedFiles\0batchOperationCompleted\0"
    "successCount\0failCount\0statusChanged\0"
    "status\0errorOccurred\0onNetworkReplyFinished\0"
    "onNetworkError\0QNetworkReply::NetworkError\0"
    "onProgressUpdate\0onReconnectTimer\0"
    "onScanTimeout"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_IntelligentFtpClient[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      23,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      18,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,  129,    2, 0x06 /* Public */,
       3,    0,  130,    2, 0x06 /* Public */,
       4,    1,  131,    2, 0x06 /* Public */,
       6,    0,  134,    2, 0x06 /* Public */,
       7,    1,  135,    2, 0x06 /* Public */,
       9,    3,  138,    2, 0x06 /* Public */,
      13,    1,  145,    2, 0x06 /* Public */,
      16,    1,  148,    2, 0x06 /* Public */,
      18,    1,  151,    2, 0x06 /* Public */,
      20,    3,  154,    2, 0x06 /* Public */,
      24,    2,  161,    2, 0x06 /* Public */,
      26,    2,  166,    2, 0x06 /* Public */,
      28,    3,  171,    2, 0x06 /* Public */,
      30,    2,  178,    2, 0x06 /* Public */,
      31,    2,  183,    2, 0x06 /* Public */,
      33,    3,  188,    2, 0x06 /* Public */,
      36,    1,  195,    2, 0x06 /* Public */,
      38,    1,  198,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      39,    0,  201,    2, 0x08 /* Private */,
      40,    1,  202,    2, 0x08 /* Private */,
      42,    0,  205,    2, 0x08 /* Private */,
      43,    0,  206,    2, 0x08 /* Private */,
      44,    0,  207,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    8,
    QMetaType::Void, QMetaType::Int, QMetaType::Int, QMetaType::QString,   10,   11,   12,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::QString,   17,
    QMetaType::Void, QMetaType::QString,   19,
    QMetaType::Void, QMetaType::QString, QMetaType::LongLong, QMetaType::LongLong,   21,   22,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,   19,   25,
    QMetaType::Void, QMetaType::QString, QMetaType::Bool,   19,   27,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   29,   21,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::Int,   29,   11,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   32,   11,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::Int,   29,   34,   35,
    QMetaType::Void, QMetaType::QString,   37,
    QMetaType::Void, QMetaType::QString,    5,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 41,    5,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void IntelligentFtpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<IntelligentFtpClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->connectionError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->connectionTimeout(); break;
        case 4: _t->scanStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->scanProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 6: _t->scanCompleted((*reinterpret_cast< const FtpScanResult(*)>(_a[1]))); break;
        case 7: _t->directoryChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 8: _t->fileDownloadStarted((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 9: _t->fileDownloadProgress((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< qint64(*)>(_a[2])),(*reinterpret_cast< qint64(*)>(_a[3]))); break;
        case 10: _t->fileDownloadCompleted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 11: _t->fileDeleteCompleted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 12: _t->fileOperationError((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3]))); break;
        case 13: _t->batchOperationStarted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 14: _t->batchOperationProgress((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 15: _t->batchOperationCompleted((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< int(*)>(_a[3]))); break;
        case 16: _t->statusChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 17: _t->errorOccurred((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 18: _t->onNetworkReplyFinished(); break;
        case 19: _t->onNetworkError((*reinterpret_cast< QNetworkReply::NetworkError(*)>(_a[1]))); break;
        case 20: _t->onProgressUpdate(); break;
        case 21: _t->onReconnectTimer(); break;
        case 22: _t->onScanTimeout(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 19:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< QNetworkReply::NetworkError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (IntelligentFtpClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::connected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::disconnected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::connectionError)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::connectionTimeout)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::scanStarted)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(int , int , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::scanProgress)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const FtpScanResult & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::scanCompleted)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::directoryChanged)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::fileDownloadStarted)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & , qint64 , qint64 );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::fileDownloadProgress)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::fileDownloadCompleted)) {
                *result = 10;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & , bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::fileDeleteCompleted)) {
                *result = 11;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & , const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::fileOperationError)) {
                *result = 12;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::batchOperationStarted)) {
                *result = 13;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::batchOperationProgress)) {
                *result = 14;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & , int , int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::batchOperationCompleted)) {
                *result = 15;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::statusChanged)) {
                *result = 16;
                return;
            }
        }
        {
            using _t = void (IntelligentFtpClient::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&IntelligentFtpClient::errorOccurred)) {
                *result = 17;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject IntelligentFtpClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_IntelligentFtpClient.data,
    qt_meta_data_IntelligentFtpClient,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *IntelligentFtpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *IntelligentFtpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_IntelligentFtpClient.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int IntelligentFtpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 23)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 23;
    }
    return _id;
}

// SIGNAL 0
void IntelligentFtpClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void IntelligentFtpClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void IntelligentFtpClient::connectionError(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void IntelligentFtpClient::connectionTimeout()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void IntelligentFtpClient::scanStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void IntelligentFtpClient::scanProgress(int _t1, int _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void IntelligentFtpClient::scanCompleted(const FtpScanResult & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void IntelligentFtpClient::directoryChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void IntelligentFtpClient::fileDownloadStarted(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}

// SIGNAL 9
void IntelligentFtpClient::fileDownloadProgress(const QString & _t1, qint64 _t2, qint64 _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void IntelligentFtpClient::fileDownloadCompleted(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}

// SIGNAL 11
void IntelligentFtpClient::fileDeleteCompleted(const QString & _t1, bool _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 11, _a);
}

// SIGNAL 12
void IntelligentFtpClient::fileOperationError(const QString & _t1, const QString & _t2, const QString & _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 12, _a);
}

// SIGNAL 13
void IntelligentFtpClient::batchOperationStarted(const QString & _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 13, _a);
}

// SIGNAL 14
void IntelligentFtpClient::batchOperationProgress(int _t1, int _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 14, _a);
}

// SIGNAL 15
void IntelligentFtpClient::batchOperationCompleted(const QString & _t1, int _t2, int _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))) };
    QMetaObject::activate(this, &staticMetaObject, 15, _a);
}

// SIGNAL 16
void IntelligentFtpClient::statusChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 16, _a);
}

// SIGNAL 17
void IntelligentFtpClient::errorOccurred(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 17, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'logindialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/logindialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logindialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_LoginDialog_t {
    QByteArrayData data[20];
    char stringdata0[278];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoginDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoginDialog_t qt_meta_stringdata_LoginDialog = {
    {
QT_MOC_LITERAL(0, 0, 11), // "LoginDialog"
QT_MOC_LITERAL(1, 12, 14), // "loginRequested"
QT_MOC_LITERAL(2, 27, 0), // ""
QT_MOC_LITERAL(3, 28, 9), // "LoginData"
QT_MOC_LITERAL(4, 38, 5), // "login"
QT_MOC_LITERAL(5, 44, 17), // "connectionTesting"
QT_MOC_LITERAL(6, 62, 18), // "credentialsCleared"
QT_MOC_LITERAL(7, 81, 15), // "loginSuccessful"
QT_MOC_LITERAL(8, 97, 2), // "ip"
QT_MOC_LITERAL(9, 100, 4), // "port"
QT_MOC_LITERAL(10, 105, 7), // "service"
QT_MOC_LITERAL(11, 113, 14), // "testConnection"
QT_MOC_LITERAL(12, 128, 16), // "clearCredentials"
QT_MOC_LITERAL(13, 145, 20), // "loadSavedCredentials"
QT_MOC_LITERAL(14, 166, 17), // "onUsernameChanged"
QT_MOC_LITERAL(15, 184, 17), // "onPasswordChanged"
QT_MOC_LITERAL(16, 202, 24), // "onSaveCredentialsToggled"
QT_MOC_LITERAL(17, 227, 7), // "enabled"
QT_MOC_LITERAL(18, 235, 18), // "onAutoLoginToggled"
QT_MOC_LITERAL(19, 254, 23) // "onTestConnectionClicked"

    },
    "LoginDialog\0loginRequested\0\0LoginData\0"
    "login\0connectionTesting\0credentialsCleared\0"
    "loginSuccessful\0ip\0port\0service\0"
    "testConnection\0clearCredentials\0"
    "loadSavedCredentials\0onUsernameChanged\0"
    "onPasswordChanged\0onSaveCredentialsToggled\0"
    "enabled\0onAutoLoginToggled\0"
    "onTestConnectionClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoginDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   74,    2, 0x06 /* Public */,
       5,    0,   77,    2, 0x06 /* Public */,
       6,    0,   78,    2, 0x06 /* Public */,
       7,    4,   79,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   88,    2, 0x0a /* Public */,
      12,    0,   89,    2, 0x0a /* Public */,
      13,    0,   90,    2, 0x0a /* Public */,
      14,    0,   91,    2, 0x08 /* Private */,
      15,    0,   92,    2, 0x08 /* Private */,
      16,    1,   93,    2, 0x08 /* Private */,
      18,    1,   96,    2, 0x08 /* Private */,
      19,    0,   99,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::Int, QMetaType::QString, 0x80000000 | 3,    8,    9,   10,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void, QMetaType::Bool,   17,
    QMetaType::Void,

       0        // eod
};

void LoginDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<LoginDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->loginRequested((*reinterpret_cast< const LoginData(*)>(_a[1]))); break;
        case 1: _t->connectionTesting(); break;
        case 2: _t->credentialsCleared(); break;
        case 3: _t->loginSuccessful((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< const LoginData(*)>(_a[4]))); break;
        case 4: _t->testConnection(); break;
        case 5: _t->clearCredentials(); break;
        case 6: _t->loadSavedCredentials(); break;
        case 7: _t->onUsernameChanged(); break;
        case 8: _t->onPasswordChanged(); break;
        case 9: _t->onSaveCredentialsToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 10: _t->onAutoLoginToggled((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 11: _t->onTestConnectionClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (LoginDialog::*)(const LoginData & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LoginDialog::loginRequested)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (LoginDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LoginDialog::connectionTesting)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (LoginDialog::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LoginDialog::credentialsCleared)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (LoginDialog::*)(const QString & , int , const QString & , const LoginData & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&LoginDialog::loginSuccessful)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject LoginDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_LoginDialog.data,
    qt_meta_data_LoginDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *LoginDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_LoginDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int LoginDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
void LoginDialog::loginRequested(const LoginData & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LoginDialog::connectionTesting()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void LoginDialog::credentialsCleared()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void LoginDialog::loginSuccessful(const QString & _t1, int _t2, const QString & _t3, const LoginData & _t4)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t3))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t4))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

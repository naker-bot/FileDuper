/****************************************************************************
** Meta object code from reading C++ file 'networkpresetmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/networkpresetmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkpresetmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkPresetManager_t {
    QByteArrayData data[18];
    char stringdata0[220];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkPresetManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkPresetManager_t qt_meta_stringdata_NetworkPresetManager = {
    {
QT_MOC_LITERAL(0, 0, 20), // "NetworkPresetManager"
QT_MOC_LITERAL(1, 21, 11), // "presetAdded"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 4), // "name"
QT_MOC_LITERAL(4, 39, 13), // "presetRemoved"
QT_MOC_LITERAL(5, 53, 13), // "presetUpdated"
QT_MOC_LITERAL(6, 67, 19), // "activePresetChanged"
QT_MOC_LITERAL(7, 87, 10), // "groupAdded"
QT_MOC_LITERAL(8, 98, 12), // "groupRemoved"
QT_MOC_LITERAL(9, 111, 12), // "groupUpdated"
QT_MOC_LITERAL(10, 124, 14), // "settingsLoaded"
QT_MOC_LITERAL(11, 139, 13), // "settingsSaved"
QT_MOC_LITERAL(12, 153, 13), // "backupCreated"
QT_MOC_LITERAL(13, 167, 8), // "filePath"
QT_MOC_LITERAL(14, 176, 15), // "importCompleted"
QT_MOC_LITERAL(15, 192, 5), // "count"
QT_MOC_LITERAL(16, 198, 12), // "onPresetUsed"
QT_MOC_LITERAL(17, 211, 8) // "autoSave"

    },
    "NetworkPresetManager\0presetAdded\0\0"
    "name\0presetRemoved\0presetUpdated\0"
    "activePresetChanged\0groupAdded\0"
    "groupRemoved\0groupUpdated\0settingsLoaded\0"
    "settingsSaved\0backupCreated\0filePath\0"
    "importCompleted\0count\0onPresetUsed\0"
    "autoSave"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkPresetManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
      11,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x06 /* Public */,
       4,    1,   82,    2, 0x06 /* Public */,
       5,    1,   85,    2, 0x06 /* Public */,
       6,    1,   88,    2, 0x06 /* Public */,
       7,    1,   91,    2, 0x06 /* Public */,
       8,    1,   94,    2, 0x06 /* Public */,
       9,    1,   97,    2, 0x06 /* Public */,
      10,    0,  100,    2, 0x06 /* Public */,
      11,    0,  101,    2, 0x06 /* Public */,
      12,    1,  102,    2, 0x06 /* Public */,
      14,    1,  105,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      16,    1,  108,    2, 0x08 /* Private */,
      17,    0,  111,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void, QMetaType::Int,   15,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

       0        // eod
};

void NetworkPresetManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkPresetManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->presetAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->presetRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->presetUpdated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->activePresetChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->groupAdded((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 5: _t->groupRemoved((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 6: _t->groupUpdated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->settingsLoaded(); break;
        case 8: _t->settingsSaved(); break;
        case 9: _t->backupCreated((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 10: _t->importCompleted((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->onPresetUsed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 12: _t->autoSave(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkPresetManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::presetAdded)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::presetRemoved)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::presetUpdated)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::activePresetChanged)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::groupAdded)) {
                *result = 4;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::groupRemoved)) {
                *result = 5;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::groupUpdated)) {
                *result = 6;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::settingsLoaded)) {
                *result = 7;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::settingsSaved)) {
                *result = 8;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::backupCreated)) {
                *result = 9;
                return;
            }
        }
        {
            using _t = void (NetworkPresetManager::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetManager::importCompleted)) {
                *result = 10;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkPresetManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_NetworkPresetManager.data,
    qt_meta_data_NetworkPresetManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkPresetManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkPresetManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkPresetManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NetworkPresetManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void NetworkPresetManager::presetAdded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkPresetManager::presetRemoved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkPresetManager::presetUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkPresetManager::activePresetChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void NetworkPresetManager::groupAdded(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void NetworkPresetManager::groupRemoved(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void NetworkPresetManager::groupUpdated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}

// SIGNAL 7
void NetworkPresetManager::settingsLoaded()
{
    QMetaObject::activate(this, &staticMetaObject, 7, nullptr);
}

// SIGNAL 8
void NetworkPresetManager::settingsSaved()
{
    QMetaObject::activate(this, &staticMetaObject, 8, nullptr);
}

// SIGNAL 9
void NetworkPresetManager::backupCreated(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 9, _a);
}

// SIGNAL 10
void NetworkPresetManager::importCompleted(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 10, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

/****************************************************************************
** Meta object code from reading C++ file 'presetmanager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/presetmanager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'presetmanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_PresetManager_t {
    QByteArrayData data[15];
    char stringdata0[190];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_PresetManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_PresetManager_t qt_meta_stringdata_PresetManager = {
    {
QT_MOC_LITERAL(0, 0, 13), // "PresetManager"
QT_MOC_LITERAL(1, 14, 20), // "newFileTypesDetected"
QT_MOC_LITERAL(2, 35, 0), // ""
QT_MOC_LITERAL(3, 36, 10), // "extensions"
QT_MOC_LITERAL(4, 47, 22), // "autoAddDialogRequested"
QT_MOC_LITERAL(5, 70, 9), // "extension"
QT_MOC_LITERAL(6, 80, 17), // "suggestedCategory"
QT_MOC_LITERAL(7, 98, 13), // "settingsSaved"
QT_MOC_LITERAL(8, 112, 14), // "presetsChanged"
QT_MOC_LITERAL(9, 127, 16), // "autoSaveSettings"
QT_MOC_LITERAL(10, 144, 16), // "analyzeFileTypes"
QT_MOC_LITERAL(11, 161, 8), // "getLogin"
QT_MOC_LITERAL(12, 170, 9), // "LoginData"
QT_MOC_LITERAL(13, 180, 4), // "host"
QT_MOC_LITERAL(14, 185, 4) // "port"

    },
    "PresetManager\0newFileTypesDetected\0\0"
    "extensions\0autoAddDialogRequested\0"
    "extension\0suggestedCategory\0settingsSaved\0"
    "presetsChanged\0autoSaveSettings\0"
    "analyzeFileTypes\0getLogin\0LoginData\0"
    "host\0port"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_PresetManager[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   49,    2, 0x06 /* Public */,
       4,    2,   52,    2, 0x06 /* Public */,
       7,    0,   57,    2, 0x06 /* Public */,
       8,    0,   58,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       9,    0,   59,    2, 0x08 /* Private */,
      10,    0,   60,    2, 0x08 /* Private */,

 // methods: name, argc, parameters, tag, flags
      11,    2,   61,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

 // methods: parameters
    0x80000000 | 12, QMetaType::QString, QMetaType::Int,   13,   14,

       0        // eod
};

void PresetManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<PresetManager *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->newFileTypesDetected((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->autoAddDialogRequested((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->settingsSaved(); break;
        case 3: _t->presetsChanged(); break;
        case 4: _t->autoSaveSettings(); break;
        case 5: _t->analyzeFileTypes(); break;
        case 6: { LoginData _r = _t->getLogin((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2])));
            if (_a[0]) *reinterpret_cast< LoginData*>(_a[0]) = std::move(_r); }  break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (PresetManager::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PresetManager::newFileTypesDetected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (PresetManager::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PresetManager::autoAddDialogRequested)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (PresetManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PresetManager::settingsSaved)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (PresetManager::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&PresetManager::presetsChanged)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject PresetManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_meta_stringdata_PresetManager.data,
    qt_meta_data_PresetManager,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *PresetManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *PresetManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_PresetManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int PresetManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
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

// SIGNAL 0
void PresetManager::newFileTypesDetected(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void PresetManager::autoAddDialogRequested(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void PresetManager::settingsSaved()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void PresetManager::presetsChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

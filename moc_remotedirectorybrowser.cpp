/****************************************************************************
** Meta object code from reading C++ file 'remotedirectorybrowser.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.17)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/remotedirectorybrowser.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'remotedirectorybrowser.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.17. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_RemoteDirectoryBrowser_t {
    QByteArrayData data[27];
    char stringdata0[420];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_RemoteDirectoryBrowser_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_RemoteDirectoryBrowser_t qt_meta_stringdata_RemoteDirectoryBrowser = {
    {
QT_MOC_LITERAL(0, 0, 22), // "RemoteDirectoryBrowser"
QT_MOC_LITERAL(1, 23, 19), // "directoriesSelected"
QT_MOC_LITERAL(2, 43, 0), // ""
QT_MOC_LITERAL(3, 44, 11), // "remotePaths"
QT_MOC_LITERAL(4, 56, 21), // "connectionEstablished"
QT_MOC_LITERAL(5, 78, 16), // "connectionFailed"
QT_MOC_LITERAL(6, 95, 5), // "error"
QT_MOC_LITERAL(7, 101, 20), // "directoryTreeUpdated"
QT_MOC_LITERAL(8, 122, 15), // "connectToServer"
QT_MOC_LITERAL(9, 138, 20), // "refreshDirectoryTree"
QT_MOC_LITERAL(10, 159, 25), // "expandSelectedDirectories"
QT_MOC_LITERAL(11, 185, 20), // "selectAllDirectories"
QT_MOC_LITERAL(12, 206, 22), // "deselectAllDirectories"
QT_MOC_LITERAL(13, 229, 20), // "addSelectedToScanner"
QT_MOC_LITERAL(14, 250, 14), // "onItemExpanded"
QT_MOC_LITERAL(15, 265, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(16, 282, 4), // "item"
QT_MOC_LITERAL(17, 287, 13), // "onItemChanged"
QT_MOC_LITERAL(18, 301, 6), // "column"
QT_MOC_LITERAL(19, 308, 20), // "onConnectionProgress"
QT_MOC_LITERAL(20, 329, 6), // "status"
QT_MOC_LITERAL(21, 336, 23), // "onDirectoryListReceived"
QT_MOC_LITERAL(22, 360, 11), // "directories"
QT_MOC_LITERAL(23, 372, 7), // "success"
QT_MOC_LITERAL(24, 380, 19), // "onCustomPathChanged"
QT_MOC_LITERAL(25, 400, 15), // "showContextMenu"
QT_MOC_LITERAL(26, 416, 3) // "pos"

    },
    "RemoteDirectoryBrowser\0directoriesSelected\0"
    "\0remotePaths\0connectionEstablished\0"
    "connectionFailed\0error\0directoryTreeUpdated\0"
    "connectToServer\0refreshDirectoryTree\0"
    "expandSelectedDirectories\0"
    "selectAllDirectories\0deselectAllDirectories\0"
    "addSelectedToScanner\0onItemExpanded\0"
    "QTreeWidgetItem*\0item\0onItemChanged\0"
    "column\0onConnectionProgress\0status\0"
    "onDirectoryListReceived\0directories\0"
    "success\0onCustomPathChanged\0showContextMenu\0"
    "pos"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_RemoteDirectoryBrowser[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      16,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   94,    2, 0x06 /* Public */,
       4,    0,   97,    2, 0x06 /* Public */,
       5,    1,   98,    2, 0x06 /* Public */,
       7,    0,  101,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    0,  102,    2, 0x0a /* Public */,
       9,    0,  103,    2, 0x0a /* Public */,
      10,    0,  104,    2, 0x0a /* Public */,
      11,    0,  105,    2, 0x0a /* Public */,
      12,    0,  106,    2, 0x0a /* Public */,
      13,    0,  107,    2, 0x0a /* Public */,
      14,    1,  108,    2, 0x08 /* Private */,
      17,    2,  111,    2, 0x08 /* Private */,
      19,    1,  116,    2, 0x08 /* Private */,
      21,    2,  119,    2, 0x08 /* Private */,
      24,    0,  124,    2, 0x08 /* Private */,
      25,    1,  125,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList,    3,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    6,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,   16,
    QMetaType::Void, 0x80000000 | 15, QMetaType::Int,   16,   18,
    QMetaType::Void, QMetaType::QString,   20,
    QMetaType::Void, QMetaType::QStringList, QMetaType::Bool,   22,   23,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QPoint,   26,

       0        // eod
};

void RemoteDirectoryBrowser::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<RemoteDirectoryBrowser *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->directoriesSelected((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 1: _t->connectionEstablished(); break;
        case 2: _t->connectionFailed((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->directoryTreeUpdated(); break;
        case 4: _t->connectToServer(); break;
        case 5: _t->refreshDirectoryTree(); break;
        case 6: _t->expandSelectedDirectories(); break;
        case 7: _t->selectAllDirectories(); break;
        case 8: _t->deselectAllDirectories(); break;
        case 9: _t->addSelectedToScanner(); break;
        case 10: _t->onItemExpanded((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 11: _t->onItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 12: _t->onConnectionProgress((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 13: _t->onDirectoryListReceived((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 14: _t->onCustomPathChanged(); break;
        case 15: _t->showContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (RemoteDirectoryBrowser::*)(const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RemoteDirectoryBrowser::directoriesSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (RemoteDirectoryBrowser::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RemoteDirectoryBrowser::connectionEstablished)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (RemoteDirectoryBrowser::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RemoteDirectoryBrowser::connectionFailed)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (RemoteDirectoryBrowser::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&RemoteDirectoryBrowser::directoryTreeUpdated)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject RemoteDirectoryBrowser::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_RemoteDirectoryBrowser.data,
    qt_meta_data_RemoteDirectoryBrowser,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *RemoteDirectoryBrowser::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *RemoteDirectoryBrowser::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_RemoteDirectoryBrowser.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int RemoteDirectoryBrowser::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 16)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 16;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 16)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 16;
    }
    return _id;
}

// SIGNAL 0
void RemoteDirectoryBrowser::directoriesSelected(const QStringList & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void RemoteDirectoryBrowser::connectionEstablished()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void RemoteDirectoryBrowser::connectionFailed(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void RemoteDirectoryBrowser::directoryTreeUpdated()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

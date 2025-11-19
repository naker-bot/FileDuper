/****************************************************************************
** Meta object code from reading C++ file 'directoryselectiondialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/directoryselectiondialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'directoryselectiondialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_DirectorySelectionDialog_t {
    QByteArrayData data[25];
    char stringdata0[407];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_DirectorySelectionDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_DirectorySelectionDialog_t qt_meta_stringdata_DirectorySelectionDialog = {
    {
QT_MOC_LITERAL(0, 0, 24), // "DirectorySelectionDialog"
QT_MOC_LITERAL(1, 25, 19), // "directoriesSelected"
QT_MOC_LITERAL(2, 45, 0), // ""
QT_MOC_LITERAL(3, 46, 5), // "local"
QT_MOC_LITERAL(4, 52, 7), // "network"
QT_MOC_LITERAL(5, 60, 22), // "networkServiceSelected"
QT_MOC_LITERAL(6, 83, 2), // "ip"
QT_MOC_LITERAL(7, 86, 4), // "port"
QT_MOC_LITERAL(8, 91, 19), // "networkServiceFound"
QT_MOC_LITERAL(9, 111, 14), // "NetworkService"
QT_MOC_LITERAL(10, 126, 7), // "service"
QT_MOC_LITERAL(11, 134, 17), // "addLocalDirectory"
QT_MOC_LITERAL(12, 152, 25), // "removeSelectedDirectories"
QT_MOC_LITERAL(13, 178, 29), // "onLocalDirectoryDoubleClicked"
QT_MOC_LITERAL(14, 208, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(15, 225, 4), // "item"
QT_MOC_LITERAL(16, 230, 29), // "onNetworkServiceDoubleClicked"
QT_MOC_LITERAL(17, 260, 20), // "showLocalContextMenu"
QT_MOC_LITERAL(18, 281, 3), // "pos"
QT_MOC_LITERAL(19, 285, 22), // "showNetworkContextMenu"
QT_MOC_LITERAL(20, 308, 22), // "updatePortScanProgress"
QT_MOC_LITERAL(21, 331, 14), // "expandAllTrees"
QT_MOC_LITERAL(22, 346, 16), // "collapseAllTrees"
QT_MOC_LITERAL(23, 363, 20), // "selectAllDirectories"
QT_MOC_LITERAL(24, 384, 22) // "deselectAllDirectories"

    },
    "DirectorySelectionDialog\0directoriesSelected\0"
    "\0local\0network\0networkServiceSelected\0"
    "ip\0port\0networkServiceFound\0NetworkService\0"
    "service\0addLocalDirectory\0"
    "removeSelectedDirectories\0"
    "onLocalDirectoryDoubleClicked\0"
    "QTreeWidgetItem*\0item\0"
    "onNetworkServiceDoubleClicked\0"
    "showLocalContextMenu\0pos\0"
    "showNetworkContextMenu\0updatePortScanProgress\0"
    "expandAllTrees\0collapseAllTrees\0"
    "selectAllDirectories\0deselectAllDirectories"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_DirectorySelectionDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   84,    2, 0x06 /* Public */,
       5,    2,   89,    2, 0x06 /* Public */,
       8,    1,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      11,    0,   97,    2, 0x08 /* Private */,
      12,    0,   98,    2, 0x08 /* Private */,
      13,    1,   99,    2, 0x08 /* Private */,
      16,    1,  102,    2, 0x08 /* Private */,
      17,    1,  105,    2, 0x08 /* Private */,
      19,    1,  108,    2, 0x08 /* Private */,
      20,    0,  111,    2, 0x08 /* Private */,
      21,    0,  112,    2, 0x08 /* Private */,
      22,    0,  113,    2, 0x08 /* Private */,
      23,    0,  114,    2, 0x08 /* Private */,
      24,    0,  115,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QStringList, QMetaType::QStringList,    3,    4,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, 0x80000000 | 14,   15,
    QMetaType::Void, QMetaType::QPoint,   18,
    QMetaType::Void, QMetaType::QPoint,   18,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void DirectorySelectionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<DirectorySelectionDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->directoriesSelected((*reinterpret_cast< const QStringList(*)>(_a[1])),(*reinterpret_cast< const QStringList(*)>(_a[2]))); break;
        case 1: _t->networkServiceSelected((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->networkServiceFound((*reinterpret_cast< const NetworkService(*)>(_a[1]))); break;
        case 3: _t->addLocalDirectory(); break;
        case 4: _t->removeSelectedDirectories(); break;
        case 5: _t->onLocalDirectoryDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 6: _t->onNetworkServiceDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1]))); break;
        case 7: _t->showLocalContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 8: _t->showNetworkContextMenu((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        case 9: _t->updatePortScanProgress(); break;
        case 10: _t->expandAllTrees(); break;
        case 11: _t->collapseAllTrees(); break;
        case 12: _t->selectAllDirectories(); break;
        case 13: _t->deselectAllDirectories(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (DirectorySelectionDialog::*)(const QStringList & , const QStringList & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DirectorySelectionDialog::directoriesSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (DirectorySelectionDialog::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DirectorySelectionDialog::networkServiceSelected)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (DirectorySelectionDialog::*)(const NetworkService & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&DirectorySelectionDialog::networkServiceFound)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject DirectorySelectionDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_DirectorySelectionDialog.data,
    qt_meta_data_DirectorySelectionDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *DirectorySelectionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DirectorySelectionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_DirectorySelectionDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int DirectorySelectionDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DirectorySelectionDialog::directoriesSelected(const QStringList & _t1, const QStringList & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void DirectorySelectionDialog::networkServiceSelected(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void DirectorySelectionDialog::networkServiceFound(const NetworkService & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

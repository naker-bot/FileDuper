/****************************************************************************
** Meta object code from reading C++ file 'hierarchicaldialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/hierarchicaldialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'hierarchicaldialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_HierarchicalDialog_t {
    QByteArrayData data[12];
    char stringdata0[144];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_HierarchicalDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_HierarchicalDialog_t qt_meta_stringdata_HierarchicalDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "HierarchicalDialog"
QT_MOC_LITERAL(1, 19, 22), // "loadDirectoriesFromFtp"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 9), // "expandAll"
QT_MOC_LITERAL(4, 53, 11), // "collapseAll"
QT_MOC_LITERAL(5, 65, 9), // "selectAll"
QT_MOC_LITERAL(6, 75, 11), // "deselectAll"
QT_MOC_LITERAL(7, 87, 13), // "onItemChanged"
QT_MOC_LITERAL(8, 101, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(9, 118, 4), // "item"
QT_MOC_LITERAL(10, 123, 6), // "column"
QT_MOC_LITERAL(11, 130, 13) // "onItemClicked"

    },
    "HierarchicalDialog\0loadDirectoriesFromFtp\0"
    "\0expandAll\0collapseAll\0selectAll\0"
    "deselectAll\0onItemChanged\0QTreeWidgetItem*\0"
    "item\0column\0onItemClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_HierarchicalDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x08 /* Private */,
       3,    0,   50,    2, 0x08 /* Private */,
       4,    0,   51,    2, 0x08 /* Private */,
       5,    0,   52,    2, 0x08 /* Private */,
       6,    0,   53,    2, 0x08 /* Private */,
       7,    2,   54,    2, 0x08 /* Private */,
      11,    2,   59,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,   10,
    QMetaType::Void, 0x80000000 | 8, QMetaType::Int,    9,   10,

       0        // eod
};

void HierarchicalDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<HierarchicalDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->loadDirectoriesFromFtp(); break;
        case 1: _t->expandAll(); break;
        case 2: _t->collapseAll(); break;
        case 3: _t->selectAll(); break;
        case 4: _t->deselectAll(); break;
        case 5: _t->onItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->onItemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject HierarchicalDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_HierarchicalDialog.data,
    qt_meta_data_HierarchicalDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *HierarchicalDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HierarchicalDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_HierarchicalDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int HierarchicalDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_WARNING_POP
QT_END_MOC_NAMESPACE

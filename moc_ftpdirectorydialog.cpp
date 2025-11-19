/****************************************************************************
** Meta object code from reading C++ file 'ftpdirectorydialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/ftpdirectorydialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ftpdirectorydialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_FtpDirectoryDialog_t {
    QByteArrayData data[12];
    char stringdata0[149];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_FtpDirectoryDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_FtpDirectoryDialog_t qt_meta_stringdata_FtpDirectoryDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "FtpDirectoryDialog"
QT_MOC_LITERAL(1, 19, 11), // "onSelectAll"
QT_MOC_LITERAL(2, 31, 0), // ""
QT_MOC_LITERAL(3, 32, 12), // "onSelectNone"
QT_MOC_LITERAL(4, 45, 11), // "onExpandAll"
QT_MOC_LITERAL(5, 57, 13), // "onCollapseAll"
QT_MOC_LITERAL(6, 71, 13), // "onItemChanged"
QT_MOC_LITERAL(7, 85, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(8, 102, 4), // "item"
QT_MOC_LITERAL(9, 107, 6), // "column"
QT_MOC_LITERAL(10, 114, 13), // "onItemClicked"
QT_MOC_LITERAL(11, 128, 20) // "updateSelectionCount"

    },
    "FtpDirectoryDialog\0onSelectAll\0\0"
    "onSelectNone\0onExpandAll\0onCollapseAll\0"
    "onItemChanged\0QTreeWidgetItem*\0item\0"
    "column\0onItemClicked\0updateSelectionCount"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_FtpDirectoryDialog[] = {

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
       6,    2,   53,    2, 0x08 /* Private */,
      10,    2,   58,    2, 0x08 /* Private */,
      11,    0,   63,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int,    8,    9,
    QMetaType::Void, 0x80000000 | 7, QMetaType::Int,    8,    9,
    QMetaType::Void,

       0        // eod
};

void FtpDirectoryDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<FtpDirectoryDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onSelectAll(); break;
        case 1: _t->onSelectNone(); break;
        case 2: _t->onExpandAll(); break;
        case 3: _t->onCollapseAll(); break;
        case 4: _t->onItemChanged((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->onItemClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 6: _t->updateSelectionCount(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject FtpDirectoryDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_FtpDirectoryDialog.data,
    qt_meta_data_FtpDirectoryDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *FtpDirectoryDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *FtpDirectoryDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_FtpDirectoryDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int FtpDirectoryDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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

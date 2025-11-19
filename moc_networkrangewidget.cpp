/****************************************************************************
** Meta object code from reading C++ file 'networkrangewidget.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/networkrangewidget.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkrangewidget.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkRangeDialog_t {
    QByteArrayData data[3];
    char stringdata0[34];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkRangeDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkRangeDialog_t qt_meta_stringdata_NetworkRangeDialog = {
    {
QT_MOC_LITERAL(0, 0, 18), // "NetworkRangeDialog"
QT_MOC_LITERAL(1, 19, 13), // "validateInput"
QT_MOC_LITERAL(2, 33, 0) // ""

    },
    "NetworkRangeDialog\0validateInput\0"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkRangeDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   19,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,

       0        // eod
};

void NetworkRangeDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkRangeDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->validateInput(); break;
        default: ;
        }
    }
    (void)_a;
}

QT_INIT_METAOBJECT const QMetaObject NetworkRangeDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_NetworkRangeDialog.data,
    qt_meta_data_NetworkRangeDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkRangeDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkRangeDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkRangeDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int NetworkRangeDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 1)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 1;
    }
    return _id;
}
struct qt_meta_stringdata_NetworkRangeWidget_t {
    QByteArrayData data[17];
    char stringdata0[245];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkRangeWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkRangeWidget_t qt_meta_stringdata_NetworkRangeWidget = {
    {
QT_MOC_LITERAL(0, 0, 18), // "NetworkRangeWidget"
QT_MOC_LITERAL(1, 19, 12), // "rangeChanged"
QT_MOC_LITERAL(2, 32, 0), // ""
QT_MOC_LITERAL(3, 33, 9), // "rangeName"
QT_MOC_LITERAL(4, 43, 16), // "customRangeAdded"
QT_MOC_LITERAL(5, 60, 5), // "range"
QT_MOC_LITERAL(6, 66, 11), // "description"
QT_MOC_LITERAL(7, 78, 13), // "rangeSelected"
QT_MOC_LITERAL(8, 92, 12), // "rangesLoaded"
QT_MOC_LITERAL(9, 105, 16), // "customRangeCount"
QT_MOC_LITERAL(10, 122, 23), // "onRangeSelectionChanged"
QT_MOC_LITERAL(11, 146, 16), // "onAddCustomRange"
QT_MOC_LITERAL(12, 163, 13), // "onRemoveRange"
QT_MOC_LITERAL(13, 177, 15), // "onRefreshRanges"
QT_MOC_LITERAL(14, 193, 22), // "onNetworkRangeDetected"
QT_MOC_LITERAL(15, 216, 12), // "NetworkRange"
QT_MOC_LITERAL(16, 229, 15) // "updateRangeList"

    },
    "NetworkRangeWidget\0rangeChanged\0\0"
    "rangeName\0customRangeAdded\0range\0"
    "description\0rangeSelected\0rangesLoaded\0"
    "customRangeCount\0onRangeSelectionChanged\0"
    "onAddCustomRange\0onRemoveRange\0"
    "onRefreshRanges\0onNetworkRangeDetected\0"
    "NetworkRange\0updateRangeList"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkRangeWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   64,    2, 0x06 /* Public */,
       4,    2,   67,    2, 0x06 /* Public */,
       7,    1,   72,    2, 0x06 /* Public */,
       8,    1,   75,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      10,    0,   78,    2, 0x08 /* Private */,
      11,    0,   79,    2, 0x08 /* Private */,
      12,    0,   80,    2, 0x08 /* Private */,
      13,    0,   81,    2, 0x08 /* Private */,
      14,    1,   82,    2, 0x08 /* Private */,
      16,    0,   85,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,    6,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::Int,    9,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 15,    5,
    QMetaType::Void,

       0        // eod
};

void NetworkRangeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkRangeWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->rangeChanged((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->customRangeAdded((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 2: _t->rangeSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->rangesLoaded((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->onRangeSelectionChanged(); break;
        case 5: _t->onAddCustomRange(); break;
        case 6: _t->onRemoveRange(); break;
        case 7: _t->onRefreshRanges(); break;
        case 8: _t->onNetworkRangeDetected((*reinterpret_cast< const NetworkRange(*)>(_a[1]))); break;
        case 9: _t->updateRangeList(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkRangeWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkRangeWidget::rangeChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkRangeWidget::*)(const QString & , const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkRangeWidget::customRangeAdded)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkRangeWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkRangeWidget::rangeSelected)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (NetworkRangeWidget::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkRangeWidget::rangesLoaded)) {
                *result = 3;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkRangeWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_NetworkRangeWidget.data,
    qt_meta_data_NetworkRangeWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkRangeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkRangeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkRangeWidget.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int NetworkRangeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}

// SIGNAL 0
void NetworkRangeWidget::rangeChanged(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkRangeWidget::customRangeAdded(const QString & _t1, const QString & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkRangeWidget::rangeSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void NetworkRangeWidget::rangesLoaded(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

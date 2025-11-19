/****************************************************************************
** Meta object code from reading C++ file 'networkpresetdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.15.18)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "include/networkpresetdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkpresetdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.15.18. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NetworkPresetListWidget_t {
    QByteArrayData data[13];
    char stringdata0[182];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkPresetListWidget_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkPresetListWidget_t qt_meta_stringdata_NetworkPresetListWidget = {
    {
QT_MOC_LITERAL(0, 0, 23), // "NetworkPresetListWidget"
QT_MOC_LITERAL(1, 24, 14), // "presetSelected"
QT_MOC_LITERAL(2, 39, 0), // ""
QT_MOC_LITERAL(3, 40, 4), // "name"
QT_MOC_LITERAL(4, 45, 19), // "presetDoubleClicked"
QT_MOC_LITERAL(5, 65, 17), // "presetContextMenu"
QT_MOC_LITERAL(6, 83, 3), // "pos"
QT_MOC_LITERAL(7, 87, 22), // "onItemSelectionChanged"
QT_MOC_LITERAL(8, 110, 19), // "onItemDoubleClicked"
QT_MOC_LITERAL(9, 130, 16), // "QTreeWidgetItem*"
QT_MOC_LITERAL(10, 147, 4), // "item"
QT_MOC_LITERAL(11, 152, 6), // "column"
QT_MOC_LITERAL(12, 159, 22) // "onContextMenuRequested"

    },
    "NetworkPresetListWidget\0presetSelected\0"
    "\0name\0presetDoubleClicked\0presetContextMenu\0"
    "pos\0onItemSelectionChanged\0"
    "onItemDoubleClicked\0QTreeWidgetItem*\0"
    "item\0column\0onContextMenuRequested"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkPresetListWidget[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       4,    1,   47,    2, 0x06 /* Public */,
       5,    2,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       7,    0,   55,    2, 0x08 /* Private */,
       8,    2,   56,    2, 0x08 /* Private */,
      12,    1,   61,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QPoint,    3,    6,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 9, QMetaType::Int,   10,   11,
    QMetaType::Void, QMetaType::QPoint,    6,

       0        // eod
};

void NetworkPresetListWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkPresetListWidget *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->presetSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->presetDoubleClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->presetContextMenu((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 3: _t->onItemSelectionChanged(); break;
        case 4: _t->onItemDoubleClicked((*reinterpret_cast< QTreeWidgetItem*(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        case 5: _t->onContextMenuRequested((*reinterpret_cast< const QPoint(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkPresetListWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetListWidget::presetSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkPresetListWidget::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetListWidget::presetDoubleClicked)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (NetworkPresetListWidget::*)(const QString & , const QPoint & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetListWidget::presetContextMenu)) {
                *result = 2;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkPresetListWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QTreeWidget::staticMetaObject>(),
    qt_meta_stringdata_NetworkPresetListWidget.data,
    qt_meta_data_NetworkPresetListWidget,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkPresetListWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkPresetListWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkPresetListWidget.stringdata0))
        return static_cast<void*>(this);
    return QTreeWidget::qt_metacast(_clname);
}

int NetworkPresetListWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTreeWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void NetworkPresetListWidget::presetSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkPresetListWidget::presetDoubleClicked(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void NetworkPresetListWidget::presetContextMenu(const QString & _t1, const QPoint & _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))), const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t2))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
struct qt_meta_stringdata_NetworkPresetEditor_t {
    QByteArrayData data[10];
    char stringdata0[132];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkPresetEditor_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkPresetEditor_t qt_meta_stringdata_NetworkPresetEditor = {
    {
QT_MOC_LITERAL(0, 0, 19), // "NetworkPresetEditor"
QT_MOC_LITERAL(1, 20, 13), // "presetChanged"
QT_MOC_LITERAL(2, 34, 0), // ""
QT_MOC_LITERAL(3, 35, 17), // "validationChanged"
QT_MOC_LITERAL(4, 53, 7), // "isValid"
QT_MOC_LITERAL(5, 61, 14), // "onInputChanged"
QT_MOC_LITERAL(6, 76, 13), // "onCidrChanged"
QT_MOC_LITERAL(7, 90, 8), // "onAddTag"
QT_MOC_LITERAL(8, 99, 11), // "onRemoveTag"
QT_MOC_LITERAL(9, 111, 20) // "onColorButtonClicked"

    },
    "NetworkPresetEditor\0presetChanged\0\0"
    "validationChanged\0isValid\0onInputChanged\0"
    "onCidrChanged\0onAddTag\0onRemoveTag\0"
    "onColorButtonClicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkPresetEditor[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       7,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   49,    2, 0x06 /* Public */,
       3,    1,   50,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   53,    2, 0x08 /* Private */,
       6,    0,   54,    2, 0x08 /* Private */,
       7,    0,   55,    2, 0x08 /* Private */,
       8,    0,   56,    2, 0x08 /* Private */,
       9,    0,   57,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NetworkPresetEditor::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkPresetEditor *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->presetChanged(); break;
        case 1: _t->validationChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 2: _t->onInputChanged(); break;
        case 3: _t->onCidrChanged(); break;
        case 4: _t->onAddTag(); break;
        case 5: _t->onRemoveTag(); break;
        case 6: _t->onColorButtonClicked(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkPresetEditor::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetEditor::presetChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkPresetEditor::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetEditor::validationChanged)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkPresetEditor::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_NetworkPresetEditor.data,
    qt_meta_data_NetworkPresetEditor,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkPresetEditor::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkPresetEditor::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkPresetEditor.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int NetworkPresetEditor::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
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
void NetworkPresetEditor::presetChanged()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NetworkPresetEditor::validationChanged(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
struct qt_meta_stringdata_NetworkPresetDialog_t {
    QByteArrayData data[23];
    char stringdata0[320];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkPresetDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkPresetDialog_t qt_meta_stringdata_NetworkPresetDialog = {
    {
QT_MOC_LITERAL(0, 0, 19), // "NetworkPresetDialog"
QT_MOC_LITERAL(1, 20, 16), // "onPresetSelected"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 4), // "name"
QT_MOC_LITERAL(4, 43, 21), // "onPresetDoubleClicked"
QT_MOC_LITERAL(5, 65, 19), // "onPresetContextMenu"
QT_MOC_LITERAL(6, 85, 3), // "pos"
QT_MOC_LITERAL(7, 89, 11), // "onAddPreset"
QT_MOC_LITERAL(8, 101, 12), // "onEditPreset"
QT_MOC_LITERAL(9, 114, 14), // "onDeletePreset"
QT_MOC_LITERAL(10, 129, 17), // "onDuplicatePreset"
QT_MOC_LITERAL(11, 147, 14), // "onExportPreset"
QT_MOC_LITERAL(12, 162, 15), // "onImportPresets"
QT_MOC_LITERAL(13, 178, 13), // "onRefreshList"
QT_MOC_LITERAL(14, 192, 13), // "onSetAsActive"
QT_MOC_LITERAL(15, 206, 16), // "onAddToFavorites"
QT_MOC_LITERAL(16, 223, 21), // "onRemoveFromFavorites"
QT_MOC_LITERAL(17, 245, 13), // "onCreateGroup"
QT_MOC_LITERAL(18, 259, 14), // "onManageGroups"
QT_MOC_LITERAL(19, 274, 19), // "onValidationChanged"
QT_MOC_LITERAL(20, 294, 7), // "isValid"
QT_MOC_LITERAL(21, 302, 8), // "onAccept"
QT_MOC_LITERAL(22, 311, 8) // "onReject"

    },
    "NetworkPresetDialog\0onPresetSelected\0"
    "\0name\0onPresetDoubleClicked\0"
    "onPresetContextMenu\0pos\0onAddPreset\0"
    "onEditPreset\0onDeletePreset\0"
    "onDuplicatePreset\0onExportPreset\0"
    "onImportPresets\0onRefreshList\0"
    "onSetAsActive\0onAddToFavorites\0"
    "onRemoveFromFavorites\0onCreateGroup\0"
    "onManageGroups\0onValidationChanged\0"
    "isValid\0onAccept\0onReject"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkPresetDialog[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      18,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,  104,    2, 0x08 /* Private */,
       4,    1,  107,    2, 0x08 /* Private */,
       5,    2,  110,    2, 0x08 /* Private */,
       7,    0,  115,    2, 0x08 /* Private */,
       8,    0,  116,    2, 0x08 /* Private */,
       9,    0,  117,    2, 0x08 /* Private */,
      10,    0,  118,    2, 0x08 /* Private */,
      11,    0,  119,    2, 0x08 /* Private */,
      12,    0,  120,    2, 0x08 /* Private */,
      13,    0,  121,    2, 0x08 /* Private */,
      14,    0,  122,    2, 0x08 /* Private */,
      15,    0,  123,    2, 0x08 /* Private */,
      16,    0,  124,    2, 0x08 /* Private */,
      17,    0,  125,    2, 0x08 /* Private */,
      18,    0,  126,    2, 0x08 /* Private */,
      19,    1,  127,    2, 0x08 /* Private */,
      21,    0,  130,    2, 0x08 /* Private */,
      22,    0,  131,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::QString, QMetaType::QPoint,    3,    6,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::Bool,   20,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NetworkPresetDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkPresetDialog *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onPresetSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->onPresetDoubleClicked((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: _t->onPresetContextMenu((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QPoint(*)>(_a[2]))); break;
        case 3: _t->onAddPreset(); break;
        case 4: _t->onEditPreset(); break;
        case 5: _t->onDeletePreset(); break;
        case 6: _t->onDuplicatePreset(); break;
        case 7: _t->onExportPreset(); break;
        case 8: _t->onImportPresets(); break;
        case 9: _t->onRefreshList(); break;
        case 10: _t->onSetAsActive(); break;
        case 11: _t->onAddToFavorites(); break;
        case 12: _t->onRemoveFromFavorites(); break;
        case 13: _t->onCreateGroup(); break;
        case 14: _t->onManageGroups(); break;
        case 15: _t->onValidationChanged((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 16: _t->onAccept(); break;
        case 17: _t->onReject(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkPresetDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_meta_stringdata_NetworkPresetDialog.data,
    qt_meta_data_NetworkPresetDialog,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkPresetDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkPresetDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkPresetDialog.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int NetworkPresetDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 18)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 18;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 18)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 18;
    }
    return _id;
}
struct qt_meta_stringdata_NetworkPresetQuickSelector_t {
    QByteArrayData data[8];
    char stringdata0[129];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NetworkPresetQuickSelector_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NetworkPresetQuickSelector_t qt_meta_stringdata_NetworkPresetQuickSelector = {
    {
QT_MOC_LITERAL(0, 0, 26), // "NetworkPresetQuickSelector"
QT_MOC_LITERAL(1, 27, 14), // "presetSelected"
QT_MOC_LITERAL(2, 42, 0), // ""
QT_MOC_LITERAL(3, 43, 4), // "name"
QT_MOC_LITERAL(4, 48, 22), // "managePresetsRequested"
QT_MOC_LITERAL(5, 71, 15), // "onPresetChanged"
QT_MOC_LITERAL(6, 87, 21), // "onManageButtonClicked"
QT_MOC_LITERAL(7, 109, 19) // "onQuickAddRequested"

    },
    "NetworkPresetQuickSelector\0presetSelected\0"
    "\0name\0managePresetsRequested\0"
    "onPresetChanged\0onManageButtonClicked\0"
    "onQuickAddRequested"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NetworkPresetQuickSelector[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x06 /* Public */,
       4,    0,   42,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void NetworkPresetQuickSelector::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<NetworkPresetQuickSelector *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->presetSelected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 1: _t->managePresetsRequested(); break;
        case 2: _t->onPresetChanged(); break;
        case 3: _t->onManageButtonClicked(); break;
        case 4: _t->onQuickAddRequested(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (NetworkPresetQuickSelector::*)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetQuickSelector::presetSelected)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (NetworkPresetQuickSelector::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetworkPresetQuickSelector::managePresetsRequested)) {
                *result = 1;
                return;
            }
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject NetworkPresetQuickSelector::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_NetworkPresetQuickSelector.data,
    qt_meta_data_NetworkPresetQuickSelector,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *NetworkPresetQuickSelector::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkPresetQuickSelector::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_NetworkPresetQuickSelector.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int NetworkPresetQuickSelector::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void NetworkPresetQuickSelector::presetSelected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void NetworkPresetQuickSelector::managePresetsRequested()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE

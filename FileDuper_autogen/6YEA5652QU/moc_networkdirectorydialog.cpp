/****************************************************************************
** Meta object code from reading C++ file 'networkdirectorydialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/networkdirectorydialog.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'networkdirectorydialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN22NetworkDirectoryDialogE_t {};
} // unnamed namespace

template <> constexpr inline auto NetworkDirectoryDialog::qt_create_metaobjectdata<qt_meta_tag_ZN22NetworkDirectoryDialogE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NetworkDirectoryDialog",
        "requestFtpSubdirectories",
        "",
        "path",
        "requestSftpSubdirectories",
        "requestSmbSubdirectories",
        "shareName",
        "requestNfsSubdirectories",
        "mountPoint",
        "onItemChanged",
        "QTreeWidgetItem*",
        "item",
        "column",
        "onItemClicked",
        "onItemDoubleClicked",
        "onSelectAll",
        "onSelectNone",
        "onExpandAll",
        "onCollapseAll",
        "loadSubdirectories",
        "parentItem"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'requestFtpSubdirectories'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'requestSftpSubdirectories'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'requestSmbSubdirectories'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { QMetaType::QString, 3 },
        }}),
        // Signal 'requestNfsSubdirectories'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 8 }, { QMetaType::QString, 3 },
        }}),
        // Slot 'onItemChanged'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(9, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onItemClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onItemDoubleClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, int)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 11 }, { QMetaType::Int, 12 },
        }}),
        // Slot 'onSelectAll'
        QtMocHelpers::SlotData<void()>(15, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onSelectNone'
        QtMocHelpers::SlotData<void()>(16, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onExpandAll'
        QtMocHelpers::SlotData<void()>(17, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onCollapseAll'
        QtMocHelpers::SlotData<void()>(18, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'loadSubdirectories'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *, const QString &)>(19, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 10, 20 }, { QMetaType::QString, 3 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NetworkDirectoryDialog, qt_meta_tag_ZN22NetworkDirectoryDialogE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NetworkDirectoryDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22NetworkDirectoryDialogE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22NetworkDirectoryDialogE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN22NetworkDirectoryDialogE_t>.metaTypes,
    nullptr
} };

void NetworkDirectoryDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NetworkDirectoryDialog *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->requestFtpSubdirectories((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->requestSftpSubdirectories((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 2: _t->requestSmbSubdirectories((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 3: _t->requestNfsSubdirectories((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 4: _t->onItemChanged((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 5: _t->onItemClicked((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 6: _t->onItemDoubleClicked((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 7: _t->onSelectAll(); break;
        case 8: _t->onSelectNone(); break;
        case 9: _t->onExpandAll(); break;
        case 10: _t->onCollapseAll(); break;
        case 11: _t->loadSubdirectories((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NetworkDirectoryDialog::*)(const QString & )>(_a, &NetworkDirectoryDialog::requestFtpSubdirectories, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkDirectoryDialog::*)(const QString & )>(_a, &NetworkDirectoryDialog::requestSftpSubdirectories, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkDirectoryDialog::*)(const QString & , const QString & )>(_a, &NetworkDirectoryDialog::requestSmbSubdirectories, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NetworkDirectoryDialog::*)(const QString & , const QString & )>(_a, &NetworkDirectoryDialog::requestNfsSubdirectories, 3))
            return;
    }
}

const QMetaObject *NetworkDirectoryDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NetworkDirectoryDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN22NetworkDirectoryDialogE_t>.strings))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int NetworkDirectoryDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void NetworkDirectoryDialog::requestFtpSubdirectories(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void NetworkDirectoryDialog::requestSftpSubdirectories(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1);
}

// SIGNAL 2
void NetworkDirectoryDialog::requestSmbSubdirectories(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void NetworkDirectoryDialog::requestNfsSubdirectories(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2);
}
QT_WARNING_POP

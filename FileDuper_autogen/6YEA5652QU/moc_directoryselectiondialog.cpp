/****************************************************************************
** Meta object code from reading C++ file 'directoryselectiondialog.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/directoryselectiondialog.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'directoryselectiondialog.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN24DirectorySelectionDialogE_t {};
} // unnamed namespace

template <> constexpr inline auto DirectorySelectionDialog::qt_create_metaobjectdata<qt_meta_tag_ZN24DirectorySelectionDialogE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "DirectorySelectionDialog",
        "directoriesSelected",
        "",
        "local",
        "network",
        "networkServiceSelected",
        "ip",
        "port",
        "networkServiceFound",
        "NetworkService",
        "service",
        "addLocalDirectory",
        "removeSelectedDirectories",
        "onLocalDirectoryDoubleClicked",
        "QTreeWidgetItem*",
        "item",
        "onNetworkServiceDoubleClicked",
        "showLocalContextMenu",
        "QPoint",
        "pos",
        "showNetworkContextMenu",
        "updatePortScanProgress",
        "expandAllTrees",
        "collapseAllTrees",
        "selectAllDirectories",
        "deselectAllDirectories"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'directoriesSelected'
        QtMocHelpers::SignalData<void(const QStringList &, const QStringList &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 3 }, { QMetaType::QStringList, 4 },
        }}),
        // Signal 'networkServiceSelected'
        QtMocHelpers::SignalData<void(const QString &, const QString &)>(5, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 6 }, { QMetaType::QString, 7 },
        }}),
        // Signal 'networkServiceFound'
        QtMocHelpers::SignalData<void(const NetworkService &)>(8, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 9, 10 },
        }}),
        // Slot 'addLocalDirectory'
        QtMocHelpers::SlotData<void()>(11, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'removeSelectedDirectories'
        QtMocHelpers::SlotData<void()>(12, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'onLocalDirectoryDoubleClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *)>(13, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Slot 'onNetworkServiceDoubleClicked'
        QtMocHelpers::SlotData<void(QTreeWidgetItem *)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 14, 15 },
        }}),
        // Slot 'showLocalContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(17, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'showNetworkContextMenu'
        QtMocHelpers::SlotData<void(const QPoint &)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Slot 'updatePortScanProgress'
        QtMocHelpers::SlotData<void()>(21, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'expandAllTrees'
        QtMocHelpers::SlotData<void()>(22, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'collapseAllTrees'
        QtMocHelpers::SlotData<void()>(23, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'selectAllDirectories'
        QtMocHelpers::SlotData<void()>(24, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'deselectAllDirectories'
        QtMocHelpers::SlotData<void()>(25, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<DirectorySelectionDialog, qt_meta_tag_ZN24DirectorySelectionDialogE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject DirectorySelectionDialog::staticMetaObject = { {
    QMetaObject::SuperData::link<QDialog::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24DirectorySelectionDialogE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24DirectorySelectionDialogE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN24DirectorySelectionDialogE_t>.metaTypes,
    nullptr
} };

void DirectorySelectionDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<DirectorySelectionDialog *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->directoriesSelected((*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QStringList>>(_a[2]))); break;
        case 1: _t->networkServiceSelected((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 2: _t->networkServiceFound((*reinterpret_cast<std::add_pointer_t<NetworkService>>(_a[1]))); break;
        case 3: _t->addLocalDirectory(); break;
        case 4: _t->removeSelectedDirectories(); break;
        case 5: _t->onLocalDirectoryDoubleClicked((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        case 6: _t->onNetworkServiceDoubleClicked((*reinterpret_cast<std::add_pointer_t<QTreeWidgetItem*>>(_a[1]))); break;
        case 7: _t->showLocalContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 8: _t->showNetworkContextMenu((*reinterpret_cast<std::add_pointer_t<QPoint>>(_a[1]))); break;
        case 9: _t->updatePortScanProgress(); break;
        case 10: _t->expandAllTrees(); break;
        case 11: _t->collapseAllTrees(); break;
        case 12: _t->selectAllDirectories(); break;
        case 13: _t->deselectAllDirectories(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (DirectorySelectionDialog::*)(const QStringList & , const QStringList & )>(_a, &DirectorySelectionDialog::directoriesSelected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (DirectorySelectionDialog::*)(const QString & , const QString & )>(_a, &DirectorySelectionDialog::networkServiceSelected, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (DirectorySelectionDialog::*)(const NetworkService & )>(_a, &DirectorySelectionDialog::networkServiceFound, 2))
            return;
    }
}

const QMetaObject *DirectorySelectionDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *DirectorySelectionDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN24DirectorySelectionDialogE_t>.strings))
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
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void DirectorySelectionDialog::directoriesSelected(const QStringList & _t1, const QStringList & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2);
}

// SIGNAL 1
void DirectorySelectionDialog::networkServiceSelected(const QString & _t1, const QString & _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void DirectorySelectionDialog::networkServiceFound(const NetworkService & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}
QT_WARNING_POP

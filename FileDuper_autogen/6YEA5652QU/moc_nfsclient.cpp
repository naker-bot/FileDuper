/****************************************************************************
** Meta object code from reading C++ file 'nfsclient.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/nfsclient.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'nfsclient.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN9NfsClientE_t {};
} // unnamed namespace

template <> constexpr inline auto NfsClient::qt_create_metaobjectdata<qt_meta_tag_ZN9NfsClientE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NfsClient",
        "connected",
        "",
        "disconnected",
        "errorOccurred",
        "message",
        "exportsListReceived",
        "QList<NfsExportInfo>",
        "exports",
        "exportMounted",
        "mountPoint",
        "exportUnmounted",
        "directoryListingReceived",
        "files",
        "onNfsProcessFinished",
        "exitCode",
        "QProcess::ExitStatus",
        "exitStatus",
        "onNfsProcessError",
        "QProcess::ProcessError",
        "error"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'connected'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'disconnected'
        QtMocHelpers::SignalData<void()>(3, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'errorOccurred'
        QtMocHelpers::SignalData<void(const QString &)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 5 },
        }}),
        // Signal 'exportsListReceived'
        QtMocHelpers::SignalData<void(const QList<NfsExportInfo> &)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 },
        }}),
        // Signal 'exportMounted'
        QtMocHelpers::SignalData<void(const QString &)>(9, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'exportUnmounted'
        QtMocHelpers::SignalData<void(const QString &)>(11, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 10 },
        }}),
        // Signal 'directoryListingReceived'
        QtMocHelpers::SignalData<void(const QList<NfsExportInfo> &)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 13 },
        }}),
        // Slot 'onNfsProcessFinished'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 15 }, { 0x80000000 | 16, 17 },
        }}),
        // Slot 'onNfsProcessError'
        QtMocHelpers::SlotData<void(QProcess::ProcessError)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 19, 20 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NfsClient, qt_meta_tag_ZN9NfsClientE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NfsClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9NfsClientE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9NfsClientE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN9NfsClientE_t>.metaTypes,
    nullptr
} };

void NfsClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NfsClient *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->connected(); break;
        case 1: _t->disconnected(); break;
        case 2: _t->errorOccurred((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 3: _t->exportsListReceived((*reinterpret_cast<std::add_pointer_t<QList<NfsExportInfo>>>(_a[1]))); break;
        case 4: _t->exportMounted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 5: _t->exportUnmounted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 6: _t->directoryListingReceived((*reinterpret_cast<std::add_pointer_t<QList<NfsExportInfo>>>(_a[1]))); break;
        case 7: _t->onNfsProcessFinished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        case 8: _t->onNfsProcessError((*reinterpret_cast<std::add_pointer_t<QProcess::ProcessError>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NfsClient::*)()>(_a, &NfsClient::connected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NfsClient::*)()>(_a, &NfsClient::disconnected, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NfsClient::*)(const QString & )>(_a, &NfsClient::errorOccurred, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NfsClient::*)(const QList<NfsExportInfo> & )>(_a, &NfsClient::exportsListReceived, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (NfsClient::*)(const QString & )>(_a, &NfsClient::exportMounted, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (NfsClient::*)(const QString & )>(_a, &NfsClient::exportUnmounted, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (NfsClient::*)(const QList<NfsExportInfo> & )>(_a, &NfsClient::directoryListingReceived, 6))
            return;
    }
}

const QMetaObject *NfsClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NfsClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN9NfsClientE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NfsClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void NfsClient::connected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NfsClient::disconnected()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void NfsClient::errorOccurred(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void NfsClient::exportsListReceived(const QList<NfsExportInfo> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void NfsClient::exportMounted(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void NfsClient::exportUnmounted(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void NfsClient::directoryListingReceived(const QList<NfsExportInfo> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}
QT_WARNING_POP

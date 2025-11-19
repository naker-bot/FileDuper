/****************************************************************************
** Meta object code from reading C++ file 'curlmulti_benchmark.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/curlmulti_benchmark.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'curlmulti_benchmark.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN18CurlMultiBenchmarkE_t {};
} // unnamed namespace

template <> constexpr inline auto CurlMultiBenchmark::qt_create_metaobjectdata<qt_meta_tag_ZN18CurlMultiBenchmarkE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "CurlMultiBenchmark",
        "benchmarkStarted",
        "",
        "testName",
        "benchmarkProgress",
        "current",
        "total",
        "benchmarkFinished",
        "BenchmarkResult",
        "result",
        "benchmarkError",
        "error",
        "onTransferStarted",
        "taskId",
        "description",
        "onTransferFinished",
        "success",
        "data",
        "onTransferError",
        "onAllTransfersFinished"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'benchmarkStarted'
        QtMocHelpers::SignalData<void(const QString &)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 3 },
        }}),
        // Signal 'benchmarkProgress'
        QtMocHelpers::SignalData<void(int, int)>(4, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 5 }, { QMetaType::Int, 6 },
        }}),
        // Signal 'benchmarkFinished'
        QtMocHelpers::SignalData<void(const BenchmarkResult &)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 8, 9 },
        }}),
        // Signal 'benchmarkError'
        QtMocHelpers::SignalData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Slot 'onTransferStarted'
        QtMocHelpers::SlotData<void(int, const QString &)>(12, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 }, { QMetaType::QString, 14 },
        }}),
        // Slot 'onTransferFinished'
        QtMocHelpers::SlotData<void(int, bool, const QString &)>(15, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 }, { QMetaType::Bool, 16 }, { QMetaType::QString, 17 },
        }}),
        // Slot 'onTransferError'
        QtMocHelpers::SlotData<void(int, const QString &)>(18, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 13 }, { QMetaType::QString, 11 },
        }}),
        // Slot 'onAllTransfersFinished'
        QtMocHelpers::SlotData<void()>(19, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<CurlMultiBenchmark, qt_meta_tag_ZN18CurlMultiBenchmarkE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject CurlMultiBenchmark::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18CurlMultiBenchmarkE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18CurlMultiBenchmarkE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN18CurlMultiBenchmarkE_t>.metaTypes,
    nullptr
} };

void CurlMultiBenchmark::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<CurlMultiBenchmark *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->benchmarkStarted((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 1: _t->benchmarkProgress((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->benchmarkFinished((*reinterpret_cast<std::add_pointer_t<BenchmarkResult>>(_a[1]))); break;
        case 3: _t->benchmarkError((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->onTransferStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 5: _t->onTransferFinished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<bool>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[3]))); break;
        case 6: _t->onTransferError((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2]))); break;
        case 7: _t->onAllTransfersFinished(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (CurlMultiBenchmark::*)(const QString & )>(_a, &CurlMultiBenchmark::benchmarkStarted, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (CurlMultiBenchmark::*)(int , int )>(_a, &CurlMultiBenchmark::benchmarkProgress, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (CurlMultiBenchmark::*)(const BenchmarkResult & )>(_a, &CurlMultiBenchmark::benchmarkFinished, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (CurlMultiBenchmark::*)(const QString & )>(_a, &CurlMultiBenchmark::benchmarkError, 3))
            return;
    }
}

const QMetaObject *CurlMultiBenchmark::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *CurlMultiBenchmark::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN18CurlMultiBenchmarkE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int CurlMultiBenchmark::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void CurlMultiBenchmark::benchmarkStarted(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1);
}

// SIGNAL 1
void CurlMultiBenchmark::benchmarkProgress(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void CurlMultiBenchmark::benchmarkFinished(const BenchmarkResult & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void CurlMultiBenchmark::benchmarkError(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}
QT_WARNING_POP

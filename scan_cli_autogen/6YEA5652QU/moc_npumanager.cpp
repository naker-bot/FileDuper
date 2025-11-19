/****************************************************************************
** Meta object code from reading C++ file 'npumanager.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.9.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/npumanager.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'npumanager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.9.1. It"
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
struct qt_meta_tag_ZN10NpuManagerE_t {};
} // unnamed namespace

template <> constexpr inline auto NpuManager::qt_create_metaobjectdata<qt_meta_tag_ZN10NpuManagerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "NpuManager",
        "hardwareDetected",
        "",
        "utilizationChanged",
        "npuLoad",
        "gpuLoad",
        "capabilityChanged",
        "NpuCapability",
        "capability",
        "available",
        "error",
        "message",
        "npuLoadChanged",
        "load",
        "gpuLoadChanged",
        "imageBatchProcessed",
        "processedImages",
        "imageFeatureExtracted",
        "ImageFeature",
        "feature",
        "duplicateGroupFound",
        "NpuDuplicateGroup",
        "group",
        "featureExtractionProgress",
        "processed",
        "total",
        "featureExtractionMessage",
        "similarityAnalysisCompleted",
        "QList<NpuDuplicateGroup>",
        "groups",
        "detectHardware",
        "updateUtilization",
        "onHardwareCheckFinished"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'hardwareDetected'
        QtMocHelpers::SignalData<void()>(1, 2, QMC::AccessPublic, QMetaType::Void),
        // Signal 'utilizationChanged'
        QtMocHelpers::SignalData<void(int, int)>(3, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 4 }, { QMetaType::Int, 5 },
        }}),
        // Signal 'capabilityChanged'
        QtMocHelpers::SignalData<void(NpuCapability, bool)>(6, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 7, 8 }, { QMetaType::Bool, 9 },
        }}),
        // Signal 'error'
        QtMocHelpers::SignalData<void(const QString &)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Signal 'npuLoadChanged'
        QtMocHelpers::SignalData<void(int)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Signal 'gpuLoadChanged'
        QtMocHelpers::SignalData<void(int)>(14, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 13 },
        }}),
        // Signal 'imageBatchProcessed'
        QtMocHelpers::SignalData<void(const QStringList &)>(15, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QStringList, 16 },
        }}),
        // Signal 'imageFeatureExtracted'
        QtMocHelpers::SignalData<void(const ImageFeature &)>(17, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 18, 19 },
        }}),
        // Signal 'duplicateGroupFound'
        QtMocHelpers::SignalData<void(const NpuDuplicateGroup &)>(20, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 21, 22 },
        }}),
        // Signal 'featureExtractionProgress'
        QtMocHelpers::SignalData<void(int, int)>(23, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 24 }, { QMetaType::Int, 25 },
        }}),
        // Signal 'featureExtractionMessage'
        QtMocHelpers::SignalData<void(const QString &)>(26, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 11 },
        }}),
        // Signal 'similarityAnalysisCompleted'
        QtMocHelpers::SignalData<void(const QList<NpuDuplicateGroup> &)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 28, 29 },
        }}),
        // Slot 'detectHardware'
        QtMocHelpers::SlotData<void()>(30, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'updateUtilization'
        QtMocHelpers::SlotData<void()>(31, 2, QMC::AccessPublic, QMetaType::Void),
        // Slot 'onHardwareCheckFinished'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<NpuManager, qt_meta_tag_ZN10NpuManagerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject NpuManager::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10NpuManagerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10NpuManagerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10NpuManagerE_t>.metaTypes,
    nullptr
} };

void NpuManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<NpuManager *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->hardwareDetected(); break;
        case 1: _t->utilizationChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 2: _t->capabilityChanged((*reinterpret_cast< std::add_pointer_t<NpuCapability>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<bool>>(_a[2]))); break;
        case 3: _t->error((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 4: _t->npuLoadChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 5: _t->gpuLoadChanged((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->imageBatchProcessed((*reinterpret_cast< std::add_pointer_t<QStringList>>(_a[1]))); break;
        case 7: _t->imageFeatureExtracted((*reinterpret_cast< std::add_pointer_t<ImageFeature>>(_a[1]))); break;
        case 8: _t->duplicateGroupFound((*reinterpret_cast< std::add_pointer_t<NpuDuplicateGroup>>(_a[1]))); break;
        case 9: _t->featureExtractionProgress((*reinterpret_cast< std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast< std::add_pointer_t<int>>(_a[2]))); break;
        case 10: _t->featureExtractionMessage((*reinterpret_cast< std::add_pointer_t<QString>>(_a[1]))); break;
        case 11: _t->similarityAnalysisCompleted((*reinterpret_cast< std::add_pointer_t<QList<NpuDuplicateGroup>>>(_a[1]))); break;
        case 12: _t->detectHardware(); break;
        case 13: _t->updateUtilization(); break;
        case 14: _t->onHardwareCheckFinished(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)()>(_a, &NpuManager::hardwareDetected, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(int , int )>(_a, &NpuManager::utilizationChanged, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(NpuCapability , bool )>(_a, &NpuManager::capabilityChanged, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(const QString & )>(_a, &NpuManager::error, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(int )>(_a, &NpuManager::npuLoadChanged, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(int )>(_a, &NpuManager::gpuLoadChanged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(const QStringList & )>(_a, &NpuManager::imageBatchProcessed, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(const ImageFeature & )>(_a, &NpuManager::imageFeatureExtracted, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(const NpuDuplicateGroup & )>(_a, &NpuManager::duplicateGroupFound, 8))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(int , int )>(_a, &NpuManager::featureExtractionProgress, 9))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(const QString & )>(_a, &NpuManager::featureExtractionMessage, 10))
            return;
        if (QtMocHelpers::indexOfMethod<void (NpuManager::*)(const QList<NpuDuplicateGroup> & )>(_a, &NpuManager::similarityAnalysisCompleted, 11))
            return;
    }
}

const QMetaObject *NpuManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NpuManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10NpuManagerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int NpuManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 15;
    }
    return _id;
}

// SIGNAL 0
void NpuManager::hardwareDetected()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void NpuManager::utilizationChanged(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void NpuManager::capabilityChanged(NpuCapability _t1, bool _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1, _t2);
}

// SIGNAL 3
void NpuManager::error(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1);
}

// SIGNAL 4
void NpuManager::npuLoadChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1);
}

// SIGNAL 5
void NpuManager::gpuLoadChanged(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void NpuManager::imageBatchProcessed(const QStringList & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1);
}

// SIGNAL 7
void NpuManager::imageFeatureExtracted(const ImageFeature & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1);
}

// SIGNAL 8
void NpuManager::duplicateGroupFound(const NpuDuplicateGroup & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}

// SIGNAL 9
void NpuManager::featureExtractionProgress(int _t1, int _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 9, nullptr, _t1, _t2);
}

// SIGNAL 10
void NpuManager::featureExtractionMessage(const QString & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 10, nullptr, _t1);
}

// SIGNAL 11
void NpuManager::similarityAnalysisCompleted(const QList<NpuDuplicateGroup> & _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 11, nullptr, _t1);
}
QT_WARNING_POP

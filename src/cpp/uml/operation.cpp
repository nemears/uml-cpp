#include "uml/operation.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/behavior.h"
#include "uml/parameter.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Operation::referencingReleased(ID id) {
    BehavioralFeature::referencingReleased(id);
    ParameterableElement::referencingReleased(id);
}

void Operation::reindexName(ID id, std::string newName) {
    BehavioralFeature::reindexName(id, newName);
    ParameterableElement::reindexName(id, newName);
}

void Operation::referenceReindexed(ID oldID, ID newID) {
    BehavioralFeature::referenceReindexed(oldID, newID);
    ParameterableElement::referenceReindexed(oldID, newID);
}

void Operation::referenceErased(ID id) {
    BehavioralFeature::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

void Operation::restoreReference(Element* el) {
    BehavioralFeature::restoreReference(el);
    ParameterableElement::restoreReference(el);
}

Set<Type, Operation>& Operation::getTypeSingleton() {
    return m_type;
}

Set<Class, Operation>& Operation::getClassSingleton() {
    return m_class;
}

Set<DataType, Operation>& Operation::getDataTypeSingleton() {
    return m_dataType;
}

Set<Interface, Operation>& Operation::getInterfaceSingleton() {
    return m_interface;
}

Set<Parameter, Operation>& Operation::getOwnedParametersSet() {
    return m_operationOwnedParameters;
}

void Operation::init() {
    m_class.subsets(m_featuringClassifier);
    m_class.subsets(m_namespace);
    m_class.opposite(&Class::getOwnedOperationsSet);
    m_class.m_signature = &Operation::getClassSingleton;
    m_dataType.subsets(m_featuringClassifier);
    m_dataType.subsets(m_namespace);
    m_dataType.opposite(&DataType::getOwnedOperationsSet);
    m_dataType.m_signature = &Operation::getDataTypeSingleton;
    m_interface.subsets(m_namespace);
    m_interface.subsets(m_featuringClassifier);
    m_interface.opposite(&Interface::getOwnedOperationsSet);
    m_interface.m_signature = &Operation::getInterfaceSingleton;
    m_operationOwnedParameters.redefines(m_ownedParameters);
    m_operationOwnedParameters.opposite(&Parameter::getOperationSingleton);
    m_operationOwnedParameters.m_signature = &Operation::getOwnedParametersSet;
}

Operation::Operation() : Element(ElementType::OPERATION) {
    init();
}

Operation::~Operation() {
    mountAndRelease();
}

TypePtr Operation::getType() const {
    return m_type.get();
}

void Operation::setType(Type* type) {
    m_type.set(type);
}

void Operation::setType(Type& type) {
    m_type.set(type);
}

void Operation::setType(ID id) {
    m_type.set(id);
}

ClassPtr Operation::getClass() const {
    return m_class.get();
}

void Operation::setClass(Class* clazz) {
    m_class.set(clazz);
}

void Operation::setClass(Class& clazz) {
    m_class.set(clazz);
}

void Operation::setClass(ID id) {
    m_class.set(id);
}

DataTypePtr Operation::getDataType() const {
    return m_dataType.get();
}

void Operation::setDataType(DataType* dataType) {
    m_dataType.set(dataType);
}

void Operation::setDataType(DataType& dataType) {
    m_dataType.set(dataType);
}

void Operation::setDataType(ID id) {
    m_dataType.set(id);
}

InterfacePtr Operation::getInterface() const {
    return m_interface.get();
}

void Operation::setInterface(Interface* interface_uml) {
    m_interface.set(interface_uml);
}

void Operation::setInterface(Interface& interface_uml) {
    m_interface.set(interface_uml);
}

void Operation::setInterface(ID id) {
    m_interface.set(id);
}

OrderedSet<Parameter, Operation>& Operation::getOwnedParameters() {
    return m_operationOwnedParameters;
}

bool Operation::isSubClassOf(ElementType eType) const {
    bool ret = BehavioralFeature::isSubClassOf(eType);

    if (!ret) {
        ret = TemplateableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = ParameterableElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::OPERATION;
    }

    return ret;
}
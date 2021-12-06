#include "uml/operation.h"
#include "uml/class.h"
#include "uml/dataType.h"
#include "uml/behavior.h"
#include "uml/uml-stable.h"

using namespace UML;

void Operation::referencingReleased(ID id) {
    BehavioralFeature::referencingReleased(id);
    m_type.release(id);
    m_class.release(id);
    m_dataType.release(id);
}

void Operation::referenceReindexed(ID oldID, ID newID) {
    BehavioralFeature::referenceReindexed(oldID, newID);
    m_type.reindex(oldID, newID);
    m_class.reindex(oldID, newID);
    m_dataType.reindex(oldID, newID);
}

void Operation::restoreReferences() {
    BehavioralFeature::restoreReferences();
    // m_type.restoreReference();
    // m_class.restoreReference();
    // m_dataType.restoreReference();
}

void Operation::referenceErased(ID id) {
    BehavioralFeature::referenceErased(id);
    m_type.eraseElement(id);
    m_class.eraseElement(id);
    m_dataType.eraseElement(id);
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

Set<Parameter, Operation>& Operation::getOwnedParametersSet() {\
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
    m_operationOwnedParameters.redefines(m_ownedParameters);
    m_operationOwnedParameters.opposite(&Parameter::getOperationSingleton);
    m_operationOwnedParameters.m_signature = &Operation::getOwnedParametersSet;
}

void Operation::copy(const Operation& rhs) {
    m_type = rhs.m_type;
    m_class = rhs.m_class;
    m_dataType = rhs.m_dataType;
}

Operation::Operation() : Element(ElementType::OPERATION) {
    init();
}

Operation::Operation(const Operation& rhs) : Element(rhs, ElementType::OPERATION) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    Feature::copy(rhs);
    BehavioralFeature::copy(rhs);
    TemplateableElement::copy(rhs);
    copy(rhs);
}

Operation::~Operation() {

}

Type* Operation::getType() {
    return m_type.get();
}

Type& Operation::getTypeRef() {
    return m_type.getRef();
}

bool Operation::hasType() const {
    return m_type.has();
}

void Operation::setType(Type* type) {
    m_type.set(type);
}

void Operation::setType(Type& type) {
    m_type.set(type);
}

Class* Operation::getClass() {
    return m_class.get();
}

Class& Operation::getClassRef() {
    return m_class.getRef();
}

ID Operation::getClassID() const {
    return m_class.id();
}

bool Operation::hasClass() const {
    return m_class.has();
}

void Operation::setClass(Class* clazz) {
    m_class.set(clazz);
}

void Operation::setClass(Class& clazz) {
    m_class.set(clazz);
}

DataType* Operation::getDataType() {
    return m_dataType.get();
}

DataType& Operation::getDataTypeRef() {
    return m_dataType.getRef();
}

ID Operation::getDataTypeID() const {
    return m_dataType.id();
}

bool Operation::hasDataType() const {
    return m_dataType.has();
}

void Operation::setDataType(DataType* dataType) {
    m_dataType.set(dataType);
}

void Operation::setDataType(DataType& dataType) {
    m_dataType.set(dataType);
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
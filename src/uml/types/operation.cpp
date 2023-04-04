#include "uml/types/operation.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/behavior.h"
#include "uml/types/parameter.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void Operation::referenceReindexed(ID newID) {
    BehavioralFeature::referenceReindexed(newID);
    ParameterableElement::referenceReindexed(newID);
}

void Operation::referenceErased(ID id) {
    BehavioralFeature::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

void Operation::restoreReference(Element* el) {
    BehavioralFeature::restoreReference(el);
    ParameterableElement::restoreReference(el);
}

void Operation::restoreReferences() {
    BehavioralFeature::restoreReferences();
    if (m_namespace->get() && !m_featuringClassifier.get()) {
        if (m_namespace->get()->isSubClassOf(ElementType::CLASSIFIER)) {
            Classifier& clazz = m_namespace->get()->as<Classifier>();
            if (clazz.getFeatures().contains(m_id)) {
                m_featuringClassifier.innerAdd(clazz);
            }
        }
    }
}

TypedSet<Type, Operation>& Operation::getTypeSingleton() {
    return m_type;
}

TypedSet<Class, Operation>& Operation::getClassSingleton() {
    return m_class;
}

TypedSet<DataType, Operation>& Operation::getDataTypeSingleton() {
    return m_dataType;
}

TypedSet<Interface, Operation>& Operation::getInterfaceSingleton() {
    return m_interface;
}

Operation::Operation() : Element(ElementType::OPERATION) {
    m_class.subsets(m_featuringClassifier);
    m_class.subsets(*m_namespace);
    m_class.opposite(&Class::getOwnedOperations);
    m_dataType.subsets(m_featuringClassifier);
    m_dataType.subsets(*m_namespace);
    m_dataType.opposite(&DataType::getOwnedOperations);
    m_interface.subsets(*m_namespace);
    m_interface.subsets(m_featuringClassifier);
    m_interface.opposite(&Interface::getOwnedOperations);
    m_operationOwnedParameters.redefines(m_ownedParameters);
    m_operationOwnedParameters.opposite(&Parameter::getOperationSingleton);
}

Operation::~Operation() {
    
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
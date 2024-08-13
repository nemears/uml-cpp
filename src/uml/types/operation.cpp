#include "uml/uml-stable.h"

using namespace UML;

void Operation::referenceErased(ID id) {
    BehavioralFeature::referenceErased(id);
    ParameterableElement::referenceErased(id);
}

void Operation::restoreReferences() {
    BehavioralFeature::restoreReferences();
    if (m_namespace->get() && !m_featuringClassifier.get()) {
        if (m_namespace->get()->is(ElementType::CLASSIFIER)) {
            Classifier& clazz = m_namespace->get()->as<Classifier>();
            if (clazz.getFeatures().contains(m_id)) {
                m_featuringClassifier.innerAdd(&clazz);
            }
        }
    }
}

Singleton<Type, Operation>& Operation::getTypeSingleton() {
    return m_type;
}

Singleton<Class, Operation>& Operation::getClassSingleton() {
    return m_class;
}

Singleton<DataType, Operation>& Operation::getDataTypeSingleton() {
    return m_dataType;
}

Singleton<Interface, Operation>& Operation::getInterfaceSingleton() {
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

void Operation::setType(TypePtr type) {
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

void Operation::setClass(ClassPtr clazz) {
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

void Operation::setDataType(DataTypePtr dataType) {
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

void Operation::setInterface(InterfacePtr interface_uml) {
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

bool Operation::is(ElementType eType) const {
    bool ret = BehavioralFeature::is(eType);

    if (!ret) {
        ret = TemplateableElement::is(eType);
    }

    if (!ret) {
        ret = ParameterableElement::is(eType);
    }

    if (!ret) {
        ret = eType == ElementType::OPERATION;
    }

    return ret;
}

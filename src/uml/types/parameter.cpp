#include "uml/uml-stable.h"

using namespace UML;

Singleton<Operation, Parameter>& Parameter::getOperationSingleton() {
    return m_operation;
}

Singleton<ValueSpecification, Parameter>& Parameter::getDefaultValueSingleton() {
    return m_defaultValue;
}

Parameter::Parameter(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    TypedElement(elementType, manager),
    ParameterableElement(elementType, manager),
    ConnectableElement(elementType, manager),
    MultiplicityElement(elementType, manager)
{
    m_operation.subsets(m_namespace);
    m_operation.opposite(&Operation::getOwnedParameters);
    m_defaultValue.subsets(m_ownedElements);
    m_parameterSets.opposite(&ParameterSet::getParameters);
}

OperationPtr Parameter::getOperation() const {
    return m_operation.get();
}

void Parameter::setOperation(OperationPtr operation) {
    m_operation.set(operation);
}

void Parameter::setOperation(Operation& operation) {
    m_operation.set(operation);
}

void Parameter::setOperation(ID id) {
    m_operation.set(id);
}

ParameterDirectionKind Parameter::getDirection() {
    return m_direction;
}

void Parameter::setDirection(ParameterDirectionKind direction) {
    if (direction == ParameterDirectionKind::RETURN || direction == ParameterDirectionKind::OUT_UML || direction == ParameterDirectionKind::INOUT) {
        if (m_operation.get()) {
            if (m_operation.get()->m_returnSpecified) {
                throw ReturnParameterException(/**m_operation.get()->getElementTypeString() + " " + **/m_operation.get().id().string());
            }
            m_operation.get()->m_returnSpecified = true;
        }
    }
    m_direction = direction;
}

ValueSpecificationPtr Parameter::getDefaultValue() const {
    return m_defaultValue.get();
}

void Parameter::setDefaultValue(ValueSpecificationPtr defaultValue) {
    m_defaultValue.set(defaultValue);
}

void Parameter::setDefaultValue(ValueSpecification& defaultValue) {
    m_defaultValue.set(defaultValue);
}

void Parameter::setDefaultValue(ID id) {
    m_defaultValue.set(id);
}

ParameterEffectKind Parameter::getEffect() const {
    return m_effect;
}

void Parameter::setEffect(ParameterEffectKind effect) {
    m_effect = effect;
}

bool Parameter::isException() const {
    return m_isException;
}

void Parameter::setIsException(bool isException) {
    m_isException = isException;
}

bool Parameter::isStream() const {
    return m_isStream;
}

void Parameter::setIsStream(bool isStream) {
    m_isStream = isStream;
}

Set<ParameterSet, Parameter>& Parameter::getParameterSets() {
    return m_parameterSets;
}

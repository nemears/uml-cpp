#include "uml/types/parameter.h"
#include "uml/types/package.h"
#include "uml/types/dataType.h"
#include "uml/types/behavior.h"
#include "uml/types/operation.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Operation, Parameter>& Parameter::getOperationSingleton() {
    return m_operation;
}

TypedSet<ValueSpecification, Parameter>& Parameter::getDefaultValueSingleton() {
    return m_defaultValue;
}

void Parameter::referenceErased(ID id) {
    ConnectableElement::referenceErased(id);
    m_parameterSets.eraseElement(id);
}

Parameter::Parameter() : Element(ElementType::PARAMETER) {
    m_operation.subsets(*m_namespace);
    m_operation.opposite(&Operation::getOwnedParameters);
    m_defaultValue.subsets(*m_ownedElements);
    m_parameterSets.opposite(&ParameterSet::getParameters);
}

Parameter::~Parameter() {

}

OperationPtr Parameter::getOperation() const {
    return m_operation.get();
}

void Parameter::setOperation(Operation* operation) {
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
                throw ReturnParameterException(m_operation.get()->getElementTypeString() + " " + m_operation.get().id().string());
            }
            m_operation.get()->m_returnSpecified = true;
        }
    }
    m_direction = direction;
}

ValueSpecificationPtr Parameter::getDefaultValue() const {
    return m_defaultValue.get();
}

void Parameter::setDefaultValue(ValueSpecification* defaultValue) {
    m_defaultValue.set(defaultValue);
}

void Parameter::setDefaultValue(ValueSpecification& defaultValue) {
    m_defaultValue.set(defaultValue);
}

void Parameter::setDefaultValue(ValueSpecificationPtr defaultValue) {
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

bool Parameter::isSubClassOf(ElementType eType) const {
    bool ret = ConnectableElement::isSubClassOf(eType);

    if (!ret) {
        ret = MultiplicityElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::PARAMETER;
    }

    return ret;
}
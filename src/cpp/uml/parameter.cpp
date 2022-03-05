#include "uml/parameter.h"
#include "uml/package.h"
#include "uml/dataType.h"
#include "uml/behavior.h"
#include "uml/operation.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<Operation, Parameter>& Parameter::getOperationSingleton() {
    return m_operation;
}

void Parameter::init() {
    m_operation.subsets(m_namespace);
    m_operation.opposite(&Operation::getOwnedParametersSet);
    m_operation.m_signature = &Parameter::getOperationSingleton;
}

Parameter::Parameter() : Element(ElementType::PARAMETER) {
    init();
}


Parameter::~Parameter() {
    mountAndRelease();
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
    if (direction == ParameterDirectionKind::RETURN || direction == ParameterDirectionKind::OUT || direction == ParameterDirectionKind::INOUT) {
        if (m_operation.get()) {
            if (m_operation.get()->m_returnSpecified) {
                throw ReturnParameterException(m_operation.get()->getElementTypeString() + " " + m_operation.get().id().string());
            }
            m_operation.get()->m_returnSpecified = true;
        }
    }
    m_direction = direction;
}

std::string Parameter::getDirectionString() {
    switch(m_direction) {
        case ParameterDirectionKind::IN : {
            return "IN";
        } case ParameterDirectionKind::OUT : {
            return "OUT";
        } case ParameterDirectionKind::INOUT : {
            return "INOUT";
        } case ParameterDirectionKind::RETURN : {
            return "RETURN";
        } default : {
            return "NONE";
        }
    }
}

void Parameter::setDirectionString(std::string& directionString) {
    if (directionString.compare("IN") == 0) {
        setDirection(ParameterDirectionKind::IN);
    } else if (directionString.compare("INOUT") == 0) {
        setDirection(ParameterDirectionKind::INOUT);
    } else if (directionString.compare("OUT") == 0) {
        setDirection(ParameterDirectionKind::OUT);
    } else if (directionString.compare("RETURN") == 0) {
        setDirection(ParameterDirectionKind::RETURN);
    } else if (directionString.compare("NONE") == 0) {
        setDirection(ParameterDirectionKind::NONE);
    } else {
        throw invalidDirectionException;
    }
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
#include "uml/parameter.h"
#include "uml/operation.h"
#include "uml/behavior.h"
#include "uml/universalFunctions.h"

using namespace std;
using namespace UML;

ElementType Parameter::getElementType() const {
    return ElementType::PARAMETER;
}

void Parameter::reindexID(ID oldID, ID newID) {
    if (!m_ownerID.isNull()) {
        if (getOwner()->isSubClassOf(ElementType::BEHAVIOR)) {
            dynamic_cast<Behavior*>(getOwner())->getParameters().reindex(oldID, newID);
        }
    }

    if (!m_operationID.isNull()) {
        if (!m_operationPtr) {
            m_operationPtr = &m_manager->get<Operation>(m_operationID);
        }
        for (auto& bhv : m_operationPtr->getMethods()) {
            bhv.getParameters().reindex(oldID, newID);
        }
    }

    NamedElement::reindexID(oldID, newID);
}

void Parameter::reindexName(string oldName, string newName) {
    if (getOwner()) {
        if (getOwner()->isSubClassOf(ElementType::BEHAVIOR)) {
            dynamic_cast<Behavior*>(getOwner())->getParameters().reindex(m_id, oldName, newName);
        }
    }

    if (getOperation()) {
        for (auto& bhv : getOperation()->getMethods()) {
            bhv.getParameters().reindex(m_id, oldName, newName);
        }
    }

    NamedElement::reindexName(oldName, newName);
}

Parameter::Parameter() {
    m_direction = ParameterDirectionKind::NONE;
    m_operationPtr = 0;
}

Parameter::Parameter(const Parameter& param) : TypedElement(param) , NamedElement(param), Element(param) {
    m_direction = param.m_direction;
    m_operationID = param.m_operationID;
    if (!param.m_manager) {
        m_operationPtr = param.m_operationPtr;
    }
}

Parameter::~Parameter() {

}

Operation* Parameter::getOperation() {
    return universalGet<Operation>(m_operationID, m_operationPtr, m_manager);
}

void Parameter::setOperation(Operation* operation) {
    if (!isSameOrNull(m_operationID, operation)) {
        if (!m_operationPtr) {
            m_operationPtr = &m_manager->get<Operation>(m_operationID);
        }
        if (m_operationPtr->getOwnedParameters().count(m_id)) {
            m_operationPtr->getOwnedParameters().remove(*this);
        }
        m_operationPtr = 0;
        m_operationID = ID::nullID();
    }

    if (operation) {
        m_operationID = operation->getID();
    }

    if (!m_manager) {
        m_operationPtr = operation;
    }
    if (operation) {
        if (!operation->getOwnedParameters().count(m_id)) {
            operation->getOwnedParameters().add(*this);
        }
    }
}

ParameterDirectionKind Parameter::getDirection() {
    return m_direction;
}

void Parameter::setDirection(ParameterDirectionKind direction) {
    if (direction == ParameterDirectionKind::RETURN || direction == ParameterDirectionKind::OUT || direction == ParameterDirectionKind::INOUT) {
        if (!m_operationID.isNull()) {
            if (!m_operationPtr) {
                m_operationPtr = &m_manager->get<Operation>(m_operationID);
            }
            if (m_operationPtr->m_returnSpecified) {
                throw ReturnParameterException(m_operationPtr->getElementTypeString() + " " + m_operationPtr->getID().string());
            }
            m_operationPtr->m_returnSpecified = true;
        }
    }
    m_direction = direction;
}

string Parameter::getDirectionString() {
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

void Parameter::setDirectionString(string& directionString) {
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

void Parameter::referencingReleased(ID id) {
    TypedElement::referencingReleased(id);
    MultiplicityElement::referencingReleased(id);
    /** TODO: operation release **/
}
#include "uml/parameter.h"
#include "uml/operation.h"
#include "uml/behavior.h"

using namespace std;
using namespace UML;

void Parameter::RemoveOperationProcedure::operator()(Operation* el) const {
    if (el->getOwnedParameters().count(m_me->getID())) {
        el->getOwnedParameters().remove(*m_me);
    }
}

void Parameter::AddOperationProcedure::operator()(Operation* el) const {
    if (!el->getOwnedParameters().count(m_me->getID())) {
        el->getOwnedParameters().add(*m_me);
    }
}

ElementType Parameter::getElementType() const {
    return ElementType::PARAMETER;
}

void Parameter::reindexID(ID oldID, ID newID) {
    /**if (!m_ownerID.isNull()) {
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

    NamedElement::reindexID(oldID, newID);**/
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

void Parameter::referenceReindexed(ID oldID, ID newID) {
    TypedElement::referenceReindexed(oldID, newID);
    MultiplicityElement::referenceReindexed(oldID, newID);
    if (m_operation.id() == oldID) {
        m_operation.reindex(oldID, newID);
    }
}

Parameter::Parameter() {
    m_direction = ParameterDirectionKind::NONE;
    m_operation.m_signature = &Parameter::m_operation;
    m_operation.m_removeProcedures.push_back(new RemoveOperationProcedure(this));
    m_operation.m_addProcedures.push_back(new AddOperationProcedure(this));
}

Parameter::Parameter(const Parameter& param) : TypedElement(param) , NamedElement(param), Element(param) {
    m_direction = param.m_direction;
    m_operation = param.m_operation;
    m_operation.m_me = this;
    m_operation.m_removeProcedures.clear();
    m_operation.m_addProcedures.clear();
    m_operation.m_removeProcedures.push_back(new RemoveOperationProcedure(this));
    m_operation.m_addProcedures.push_back(new AddOperationProcedure(this));
}

Parameter::~Parameter() {

}

Operation* Parameter::getOperation() {
    return m_operation.get();
}

Operation& Parameter::getOperationRef() {
    return m_operation.getRef();
}

bool Parameter::hasOperation() const {
    return m_operation.has();
}

void Parameter::setOperation(Operation* operation) {
    m_operation.set(operation);
}

void Parameter::setOperation(Operation& operation) {
    m_operation.set(operation);
}

ParameterDirectionKind Parameter::getDirection() {
    return m_direction;
}

void Parameter::setDirection(ParameterDirectionKind direction) {
    if (direction == ParameterDirectionKind::RETURN || direction == ParameterDirectionKind::OUT || direction == ParameterDirectionKind::INOUT) {
        if (m_operation.has()) {
            if (m_operation.getRef().m_returnSpecified) {
                throw ReturnParameterException(m_operation.getRef().getElementTypeString() + " " + m_operation.id().string());
            }
            m_operation.getRef().m_returnSpecified = true;
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
    if (m_operation.id() == id) {
        m_operation.release();
    }
}
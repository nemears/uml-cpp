#include "uml/parameter.h"
#include "uml/uml-stable.h"

using namespace UML;

void Parameter::referenceReindexed(ID oldID, ID newID) {
    TypedElement::referenceReindexed(oldID, newID);
    MultiplicityElement::referenceReindexed(oldID, newID);
    m_operation.reindex(oldID, newID);
    // m_behavior.reindex(oldID, newID);
}

void Parameter::referencingReleased(ID id) {
    TypedElement::referencingReleased(id);
    MultiplicityElement::referencingReleased(id);
    m_operation.release(id);
    // m_behavior.release(id);
}

void Parameter::restoreReferences() {
    TypedElement::restoreReferences();
    MultiplicityElement::restoreReferences();
    // m_operation.restoreReference();
    // m_behavior.restoreReference();
}

void Parameter::referenceErased(ID id) {
    TypedElement::referenceErased(id);
    MultiplicityElement::referenceErased(id);
    m_operation.eraseElement(id);
    // m_behavior.elementErased(id);
}

Set<Operation, Parameter>& Parameter::getOperationSingleton() {
    return m_operation;
}

void Parameter::init() {
    m_operation.subsets(m_namespace);
    m_operation.opposite(&Operation::getOwnedParametersSet);
    m_operation.m_signature = &Parameter::getOperationSingleton;
}

void Parameter::copy(const Parameter& rhs) {
    m_operation = rhs.m_operation;
    m_direction = rhs.m_direction;
}

Parameter::Parameter() : Element(ElementType::PARAMETER) {
    init();
}

Parameter::Parameter(const Parameter& rhs) : Element(rhs, ElementType::PARAMETER) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    TypedElement::copy(rhs);
    MultiplicityElement::copy(rhs);
    copy(rhs);
}

Parameter::~Parameter() {

}

Operation* Parameter::getOperation() {
    return m_operation.get();
}

Operation& Parameter::getOperationRef() {
    return m_operation.getRef();
}

ID Parameter::getOperationID() const {
    return m_operation.id();
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
        // if (m_operation.has()) {
        //     if (m_operation.getRef().m_returnSpecified) {
        //         throw ReturnParameterException(m_operation.getRef().getElementTypeString() + " " + m_operation.id().string());
        //     }
        //     m_operation.getRef().m_returnSpecified = true;
        // }
    }
    m_direction = direction;
    updateCopiesScalar(direction, &Parameter::m_direction);
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
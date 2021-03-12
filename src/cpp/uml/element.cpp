#include "uml/element.h"
#include "uml/sequence.h"

using namespace UML;

// Constructor
Element::Element() {
    m_id = boost::uuids::random_generator()();
    m_owner = NULL;
    m_ownedElements = new Sequence<Element>;
}

// Destructor
Element::~Element() {
    delete m_ownedElements;
}

void Element::setID(string id) {
    if (UML::isValidUUID4(id)) {
        m_id = boost::lexical_cast<boost::uuids::uuid>(id);
    } else {
        throw InvalidID_Exception();
    }
}

void Element::setID(boost::uuids::uuid id) {
    m_id = id;
}

Element* Element::getOwner() {
    return m_owner;
}

void Element::setOwner(Element* owner) {
    this->m_owner = owner;
}

Sequence<>& Element::getOwnedElements() {
    return *m_ownedElements;
}

ElementType Element::getElementType() {
    return ElementType::ELEMENT;
}

bool Element::isSubClassOf(ElementType eType) {

    ElementType myType = getElementType();

    if (myType == eType) {
        return true;
    }

    switch (eType) {
        case ElementType::ACTION : {
            return myType == ElementType::CALL_BEHAVIOR_ACTION || myType == ElementType::CREATE_OBJECT_ACTION;
        }
        case ElementType::ACTIVITY_EDGE : {
            return myType == ElementType::CONTROL_FLOW || myType == ElementType::OBJECT_FLOW;
        }
        case ElementType::ACTIVITY_NODE : {
            return myType == ElementType::CALL_BEHAVIOR_ACTION || myType == ElementType::CREATE_OBJECT_ACTION
                   || myType == ElementType::DECISION_NODE || myType == ElementType::FINAL_NODE || myType == ElementType::FORK_NODE
                   || myType == ElementType::INITIAL_NODE || myType == ElementType::INPUT_PIN || myType == ElementType::JOIN_NODE
                   || myType == ElementType::MERGE_NODE || myType == ElementType::OBJECT_NODE || myType == ElementType::OUTPUT_PIN
                   || myType == ElementType::PARAMETER_NODE || myType == ElementType::PIN;
        }
        case ElementType::BEHAVIOR : {
            return myType == ElementType::ACTIVITY || myType == ElementType::OPAQUE_BEHAVIOR;
        }
        case ElementType::CLASS : {
            return myType == ElementType::BEHAVIOR || myType == ElementType::ACTIVITY || myType == ElementType::OPAQUE_BEHAVIOR;
        }
        case ElementType::CLASSIFIER : {
            return myType == ElementType::BEHAVIOR || myType == ElementType::ACTIVITY || myType == ElementType::OPAQUE_BEHAVIOR
                   || myType == ElementType::CLASS || myType == ElementType::PRIMITIVE_TYPE;
        }
        case ElementType::ELEMENT : {
            return true;
        }
        case ElementType::MULTIPLICITY_ELEMENT : {
            return myType == ElementType::PROPERTY || myType == ElementType::PARAMETER || myType == ElementType::PIN;
        }
        case ElementType::NAMED_ELEMENT : {
            // xor it
            return !(myType != ElementType::ELEMENT) != !(myType == ElementType::SLOT);
        }
        case ElementType::NAMESPACE : {
            return myType == ElementType::MODEL;
        }
        case ElementType::OBJECT_NODE : {
            return myType == ElementType::INPUT_PIN || myType == ElementType::OUTPUT_PIN || myType == ElementType::PIN
                   || myType == ElementType::PARAMETER_NODE;
        }
        case ElementType::STRUCTURAL_FEATURE : {
            return myType != ElementType::PROPERTY;
        }
        case ElementType::TYPE : {
            return myType == ElementType::BEHAVIOR || myType == ElementType::ACTIVITY || myType == ElementType::OPAQUE_BEHAVIOR
                   || myType == ElementType::CLASS || myType == ElementType::PRIMITIVE_TYPE || myType == ElementType::CLASSIFIER;
        }
        case ElementType::TYPED_ELEMENT : {
            return myType == ElementType::INSTANCE_VALUE || myType == ElementType::LITERAL_BOOL || myType == ElementType::LITERAL_INT
                   || myType == ElementType::LITERAL_REAL || myType == ElementType::LITERAL_STRING || myType == ElementType::OBJECT_NODE
                   || myType == ElementType::INPUT_PIN || myType == ElementType::OUTPUT_PIN || myType == ElementType::PIN
                   || myType == ElementType::EXPRESSION || myType == ElementType::PARAMETER || myType == ElementType::PARAMETER_NODE
                   || myType == ElementType::PROPERTY || myType == ElementType::VALUE_SPECIFICATION;
        }
        case ElementType::VALUE_SPECIFICATION : {
            return myType == ElementType::INSTANCE_VALUE || myType == ElementType::LITERAL_BOOL || myType == ElementType::LITERAL_INT
                   || myType == ElementType::LITERAL_REAL || myType == ElementType::LITERAL_STRING || myType == ElementType::EXPRESSION;
        }
    }

    return false;
}

string Element::getElementTypeString() {
    switch(getElementType()) {
        case ElementType::ACTION : {
            return "ACTION";
        }
        case ElementType::ACTIVITY : {
            return "ACTIVITY";
        }
        case ElementType::ACTIVITY_EDGE : {
            return "ACTIVITY_EDGE";
        }
        case ElementType::ACTIVITY_NODE : {
            return "ACTIVITY_NODE";
        }
        case ElementType::BEHAVIOR : {
            return "BEHAVIOR";
        }
        case ElementType::CALL_BEHAVIOR_ACTION : {
            return "CALL_BEHAVIOR_ACTION";
        }
        case ElementType::CLASS : {
            return "CLASS";
        }
        case ElementType::CLASSIFIER : {
            return "CLASSIFIER";
        }
        case ElementType::CONTROL_FLOW : {
            return "CONTROL_FLOW";
        }
        case ElementType::CREATE_OBJECT_ACTION : {
            return "CREATE_OBJECT_ACTION";
        }
        case ElementType::DECISION_NODE : {
            return "DECISION_NODE";
        }
        case ElementType::ELEMENT : {
            return "ELEMENT";
        }
        case ElementType::EXPRESSION : {
            return "EXPRESSION";
        }
        case ElementType::FINAL_NODE : {
            return "FINAL_NODE";
        }
        case ElementType::FORK_NODE : {
            return "FORK_NODE";
        }
        case ElementType::INITIAL_NODE : {
            return "INITIAL_NODE";
        }
        case ElementType::INPUT_PIN : {
            return "INPUT_PIN";
        }
        case ElementType::INSTANCE_SPECIFICATION : {
            return "INSTANCE_SPECIFICATION";
        }
        case ElementType::INSTANCE_VALUE : {
            return "INSTANCE_VALUE";
        }
        case ElementType::JOIN_NODE : {
            return "JOIN_NODE";
        }
        case ElementType::LITERAL_BOOL : {
            return "LITERAL_BOOL";
        }
        case ElementType::LITERAL_INT : {
            return "LITERAL_INT";
        }
        case ElementType::LITERAL_REAL : {
            return "LITERAL_REAL";
        }
        case ElementType::LITERAL_STRING : {
            return "LITERAL_STRING";
        }
        case ElementType::MERGE_NODE : {
            return "MERGE_NODE";
        }
        case ElementType::MODEL : {
            return "MODEL";
        }
        case ElementType::MULTIPLICITY_ELEMENT : {
            return "MULTIPLICITY_ELEMENT";
        }
        case ElementType::NAMED_ELEMENT : {
            return "NAMED_ELEMENT";
        }
        case ElementType::NAMESPACE : {
            return "NAMESPACE";
        }
        case ElementType::OBJECT_FLOW : {
            return "OBJECT_FLOW";
        }
        case ElementType::OBJECT_NODE : {
            return "OBJECT_NODE";
        }
        case ElementType::OPAQUE_BEHAVIOR : {
            return "OPAQUE_BEHAVIOR";
        }
        case ElementType::OPERATION : {
            return "OPERATION";
        }
        case ElementType::OUTPUT_PIN : {
            return "OUTPUT_PIN";
        }
        case ElementType::PARAMETER : {
            return "PARAMETER";
        }
        case ElementType::PARAMETER_NODE : {
            return "PARAMETER_NODE";
        }
        case ElementType::PIN : {
            return "PIN";
        }
        case ElementType::PRIMITIVE_TYPE : {
            return "PRIMITIVE_TYPE";
        }
        case ElementType::PROPERTY : {
            return "PROPERTY";
        }
        case ElementType::SLOT : {
            return "SLOT";
        }
        case ElementType::STRUCTURAL_FEATURE : {
            return "STRUCTURAL_FEATURE";
        }
        case ElementType::TYPE : {
            return "TYPE";
        }
        case ElementType::TYPED_ELEMENT : {
            return "TYPED_ELEMENT";
        }
        case ElementType::VALUE_SPECIFICATION : {
            return "VALUE_SPECIFICATION";
        }
        default : {
            return "NOT_SET";
        }
    }
}

string Element::getIDstring() {
    return boost::lexical_cast<string>(m_id);
}
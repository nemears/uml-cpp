#include "uml/types/element.h"
#include "uml/umlPtr.h"
#include "uml/types/comment.h"
#include "uml/types/instanceSpecification.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/types/dataType.h"
#include "uml/set/singleton.h"

namespace UML {

bool isValidID(std::string strn) {
    return std::regex_match (strn, std::regex("(?:[A-Za-z0-9_&]{28})"));
}

void Element::setReference(Element* referencing) {
    if (m_node->m_references.count(referencing->getID())) {
        m_node->m_references[referencing->getID()].count++;
    } else {
        m_node->m_references[referencing->getID()] = ManagerNode::NodeReference{referencing->m_node, 1};
    }
}

void Element::setReference(ID id) {
    if (m_node->m_references.count(id)) {
        m_node->m_references[id].count++;
    } else {
        // TODO maybe check if loaded? cause this can be called when the element of the id is not released
        m_node->m_references[id] = ManagerNode::NodeReference{0, 1};
    }
}

void Element::removeReference(ID referencing) {
    if (m_node->m_references[referencing].count > 1) {
        m_node->m_references[referencing].count--;
    } else {
        m_node->m_references.erase(referencing);
    }
}

void Element::referenceReindexed(ID newID) {
    m_ownedElements->reindex(newID);
    m_owner->reindex(newID);
    m_ownedComments->reindex(newID);
    m_appliedStereotype->reindex(newID);
}

void Element::restoreReferences() {
    // nothing
}

void Element::restoreReference(Element* el) {
    if (m_node->m_references.count(el->getID())) {
        m_node->m_references[el->getID()].node = el->m_node;
    } else {
        throw ManagerStateException("Bad state, should have reference if we are restoring a reference");
    }
    // m_ownedElements->restore(el);
    // m_owner->restore(el);
}

void Element::referenceErased(ID id) {
    m_owner->eraseElement(id);
    m_ownedElements->eraseElement(id);
    m_ownedComments->eraseElement(id);
    m_appliedStereotype->eraseElement(id);
}

SetLock Element::lockEl(Element& el) {
    return m_manager->lockEl(el);
}

TypedSet<Element, Element>& Element::getOwnerSingleton() {
    return *m_owner;
}

Element::Element(ElementType elementType) : m_elementType(elementType) {
    m_manager = 0;
    m_node = 0;
    m_id = ID::randomID();

    m_owner = new CustomSingleton<Element, Element>(this);
    m_owner->opposite(&Element::getOwnedElements);

    m_ownedElements = new CustomReadOnlySet<Element, Element>(this);
    m_ownedElements->opposite(&Element::getOwnerSingleton);

    m_ownedComments = new CustomSet<Comment, Element>(this);
    m_ownedComments->subsets(*m_ownedElements);

    m_appliedStereotype = new CustomSet<InstanceSpecification, Element>(this);
    m_appliedStereotype->subsets(*m_ownedElements);
}

Element::~Element() {
    delete m_appliedStereotype;
    delete m_ownedComments;
    delete m_owner;
    delete m_ownedElements;
}

void Element::setID(std::string id) {
    if (UML::isValidID(id)) {
        setID(ID::fromString(id));
    } else {
        throw InvalidID_Exception();
    }
}

void Element::setID(ID id) {
    m_manager->reindex(m_id, id);
    m_id = id;
}

ElementType Element::getElementType() const {
    return m_elementType;
}

bool Element::isSubClassOf(ElementType eType) const {
    return eType == ElementType::ELEMENT;
}

std::string Element::elementTypeToString(ElementType eType) {
    switch(eType) {
        case ElementType::ABSTRACTION : {
            return "ABSTRACTION";
        }
        case ElementType::ACTION : {
            return "ACTION";
        }
        case ElementType::ACTION_INPUT_PIN : {
            return "ACTION_INPUT_PIN";
        }
        case ElementType::ACTIVITY : {
            return "ACTIVITY";
        }
        case ElementType::ACTIVITY_EDGE : {
            return "ACTIVITY_EDGE";
        }
        case ElementType::ACTIVITY_FINAL_NODE : {
            return "ACTIVITY_FINAL_NODE";
        }
        case ElementType::ACTIVITY_GROUP : {
            return "ACTIVITY_GROUP";
        }
        case ElementType::ACTIVITY_NODE : {
            return "ACTIVITY_NODE";
        }
        case ElementType::ACTIVITY_PARAMETER_NODE : {
            return "ACTIVITY_PARAMETER_NODE";
        }
        case ElementType::ACTIVITY_PARTITION : {
            return "ACTIVITY_PARTITION";
        }
        case ElementType::ARTIFACT : {
            return "ARTIFACT";
        }
        case ElementType::ASSOCIATION : {
            return "ASSOCIATION";
        }
        case ElementType::BEHAVIOR : {
            return "BEHAVIOR";
        }
        case ElementType::BEHAVIORAL_FEATURE : {
            return "BEHAVIOR_FEATURE";
        }
        case ElementType::BEHAVIORED_CLASSIFIER : {
            return "BEHAVIORED_CLASSIFIER";
        }
        case ElementType::CALL_ACTION : {
            return "CALL ACTION";
        }
        case ElementType::CALL_BEHAVIOR_ACTION : {
            return "CALL_BEHAVIOR_ACTION";
        }
        case ElementType::CENTRAL_BUFFER_NODE : {
            return "CENTRAL_BUFFER_NODE";
        }
        case ElementType::CLASS : {
            return "CLASS";
        }
        case ElementType::CLASSIFIER : {
            return "CLASSIFIER";
        }
        case ElementType::CLASSIFIER_TEMPLATE_PARAMETER : {
            return "CLASSIFIER_TEMPLATE_PARAMETER";
        }
        case ElementType::COMMENT : {
            return "COMMENT";
        }
        case ElementType::CONNECTABLE_ELEMENT : {
            return "CONNECTABLE_ELEMENT";
        }
        case ElementType::CONNECTOR : {
            return "CONNECTOR";
        }
        case ElementType::CONNECTOR_END : {
            return "CONNECTOR_END";
        }
        case ElementType::CONSTRAINT : {
            return "CONSTRAINT";
        }
        case ElementType::CONTROL_FLOW : {
            return "CONTROL_FLOW";
        }
        case ElementType::CONTROL_NODE : {
            return "CONTROL_NODE";
        }
        case ElementType::CREATE_OBJECT_ACTION : {
            return "CREATE_OBJECT_ACTION";
        }
        case ElementType::DATA_STORE_NODE : {
            return "DATA_STORE_NODE";
        }
        case ElementType::DATA_TYPE : {
            return "DATA_TYPE";
        }
        case ElementType::DECISION_NODE : {
            return "DECISION_NODE";
        }
        case ElementType::DEPENDENCY : {
            return "DEPENDENCY";
        }
        case ElementType::DEPLOYED_ARTIFACT : {
            return "DEPLOYED_ARTIFACT";
        }
        case ElementType::DEPLOYMENT : {
            return "DEPLOYMENT";
        }
        case ElementType::DEPLOYMENT_TARGET : {
            return "DEPLOYMENT_TARGET";
        }
        case ElementType::DIRECTED_RELATIONSHIP : {
            return "DIRECTED_RELATIONSHIP";
        }
        case ElementType::ELEMENT : {
            return "ELEMENT";
        }
        case ElementType::ELEMENT_IMPORT : {
            return "ELEMENT_IMPORT";
        }
        case ElementType::ENCAPSULATED_CLASSIFIER : {
            return "ENCAPSULATED_CLASSIFIER";
        }
        case ElementType::ENUMERATION : {
            return "ENUMERATION";
        }
        case ElementType::ENUMERATION_LITERAL : {
            return "ENUMERATION_LITERAL";
        }
        case ElementType::EXCEPTION_HANDLER : {
            return "EXCEPTION_HANDLER";
        }
        case ElementType::EXECUTABLE_NODE : {
            return "EXECUTABLE_NODE";
        }
        case ElementType::EXPRESSION : {
            return "EXPRESSION";
        }
        case ElementType::EXTENSION : {
            return "EXTENSION";
        }
        case ElementType::EXTENSION_END : {
            return "EXTENSION_END";
        }
        case ElementType::FEATURE : {
            return "FEATURE";
        }
        case ElementType::FINAL_NODE : {
            return "FINAL_NODE";
        }
        case ElementType::FLOW_FINAL_NODE : {
            return "FLOW_FINAL_NODE";
        }
        case ElementType::FORK_NODE : {
            return "FORK_NODE";
        }
        case ElementType::GENERALIZATION : {
            return "GENERALIZATION";
        }
        case ElementType::GENERALIZATION_SET : {
            return "GENERALIZATION_SET";
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
        case ElementType::INTERFACE_UML : {
            return "INTERFACE";
        }
        case ElementType::INTERFACE_REALIZATION : {
            return "INTERFACE_REALIZATION";
        }
        case ElementType::INTERRUPTIBLE_ACTIVITY_REGION : {
            return "INTERRUPTIBLE_ACTIVITY_REGION";
        }
        case ElementType::INVOCATION_ACTION : {
            return "INVOCATION_ACTION";
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
        case ElementType::LITERAL_NULL : {
            return "LITERAL_NULL";
        }
        case ElementType::LITERAL_REAL : {
            return "LITERAL_REAL";
        }
        case ElementType::LITERAL_SPECIFICATION : {
            return "LITERAL_SPECIFICATION";
        }
        case ElementType::LITERAL_STRING : {
            return "LITERAL_STRING";
        }
        case ElementType::LITERAL_UNLIMITED_NATURAL : {
            return "LITERAL_UNLIMITED_NATURAL";
        }
        case ElementType::MANIFESTATION : {
            return "MANIFESTATION";
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
        case ElementType::OPAQUE_ACTION : {
            return "OPAQUE_ACTION";
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
        case ElementType::PACKAGE : {
            return "PACKAGE";
        }
        case ElementType::PACKAGEABLE_ELEMENT : {
            return "PACKAGEABLE_ELEMENT";
        }
        case ElementType::PACKAGE_IMPORT : {
            return "PACKAGE_IMPORT";
        }
        case ElementType::PACKAGE_MERGE : {
            return "PACKAGE_MERGE";
        }
        case ElementType::PARAMETER : {
            return "PARAMETER";
        }
        case ElementType::PARAMETERABLE_ELEMENT : {
            return "PARAMETERABLE_ELEMENT";
        }
        case ElementType::PARAMETER_SET : {
            return "PARAMETER_SET";
        }
        case ElementType::PIN : {
            return "PIN";
        }
        case ElementType::PORT : {
            return "PORT";
        }
        case ElementType::PRIMITIVE_TYPE : {
            return "PRIMITIVE_TYPE";
        }
        case ElementType::PROFILE : {
            return "PROFILE";
        }
        case ElementType::PROFILE_APPLICATION : {
            return "PROFILE_APPLICATION";
        }
        case ElementType::PROPERTY : {
            return "PROPERTY";
        }
        case ElementType::REALIZATION : {
            return "REALIZATION";
        }
        case ElementType::RECEPTION : {
            return "RECEPTION";
        }
        case ElementType::REDEFINABLE_ELEMENT : {
            return "REDEFINABLE_ELEMENT";
        }
        case ElementType::REDEFINABLE_TEMPLATE_SIGNATURE : {
            return "REDEFINABLE_TEMPLATE_SIGNATURE";
        }
        case ElementType::RELATIONSHIP : {
            return "RELATIONSHIP";
        }
        case ElementType::SIGNAL : {
            return "SIGNAL";
        }
        case ElementType::SLOT : {
            return "SLOT";
        }
        case ElementType::STEREOTYPE : {
            return "STEREOTYPE";
        }
        case ElementType::STRUCTURAL_FEATURE : {
            return "STRUCTURAL_FEATURE";
        }
        case ElementType::STRUCTURED_CLASSIFIER : {
            return "STRUCTURED_CLASSIFIER";
        }
        case ElementType::TEMPLATEABLE_ELEMENT : {
            return "TEMPLATEABLE_ELEMENT";
        }
        case ElementType::TEMPLATE_BINDING : {
            return "TEMPLATE_BINDING";
        }
        case ElementType::TEMPLATE_PARAMETER : {
            return "TEMPLATE_PARAMETER";
        }
        case ElementType::TEMPLATE_PARAMETER_SUBSTITUTION : {
            return "TEMPLATE_PARAMETER_SUBSTITUTION";
        }
        case ElementType::TEMPLATE_SIGNATURE : {
            return "TEMPLATE_SIGNATURE";
        }
        case ElementType::TYPE : {
            return "TYPE";
        }
        case ElementType::TYPED_ELEMENT : {
            return "TYPED_ELEMENT";
        }
        case ElementType::USAGE : {
            return "USAGE";
        }
        case ElementType::VALUE_PIN : {
            return "VALUE_PIN";
        }
        case ElementType::VALUE_SPECIFICATION : {
            return "VALUE_SPECIFICATION";
        }
        default : {
            return "NOT_SET";
        }
    }
}

std::string Element::getElementTypeString() const {
    return elementTypeToString(getElementType());
}

ID Element::getID() const {
    return m_id;
}

ElementPtr Element::getOwner() const {
    return m_owner->get();
}

void Element::setOwner(Element* owner) {
    m_owner->set(owner);
}

void Element::setOwnerByID(ID id) {
   m_owner->set(id);
}

ReadOnlySet<Element, Element>& Element::getOwnedElements() {
    return *m_ownedElements;
}

Set<InstanceSpecification, Element>& Element::getAppliedStereotypes() {
    return *m_appliedStereotype;
}

Set<Comment, Element>& Element::getOwnedComments() {
    return *m_ownedComments;
}

// void SetReferenceFunctor::operator()(Element& el) const {
//     el.setReference(&m_el);
// }

// void RemoveReferenceFunctor::operator()(Element& el) const {
//     el.removeReference(m_el.getID());
// }

void Element::setOwner(ID id) {
    m_owner->innerAdd(id);
    m_node->setReference(id);
}

}
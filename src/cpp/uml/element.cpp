#include "uml/element.h"
#include "uml/umlManager.h"
#include "uml/sequence.h"
#include "uml/relationship.h"
#include "uml/elementFunctors.h"
#include "uml/directedRelationship.h"
#include "uml/comment.h"
#include "uml/instanceSpecification.h"

using namespace std;
using namespace UML;

void SetOwnerFunctor::operator()(Element& el) const{
    if (el.getOwner() != m_el) {
        el.setOwner(m_el);
    }
}

void AddRelationshipFunctor::operator()(Element& el) const {
    if (!dynamic_cast<Relationship&>(el).getRelatedElements().count(m_el->getID())) {
        dynamic_cast<Relationship&>(el).getRelatedElements().internalAdd(*m_el);
    }
}

void RemoveRelationshipFunctor::operator()(Element& el) const {
    if (dynamic_cast<Relationship&>(el).getRelatedElements().count(m_el->getID())) {
        dynamic_cast<Relationship&>(el).getRelatedElements().internalRemove(*m_el);
    }
}

void RemoveOwnerFunctor::operator()(Element& el) const {
    if (el.getOwner() == m_el) {
        el.setOwner(0);
    }
}

void ReadOnlySequenceFunctor::operator()(Element& el) const {
    throw ReadOnlySequenceException(m_el->getID().string(), m_name);
}

void AddDirectedRelationshipFunctor::operator()(Element& el) const {
    if (!m_el->getRelationships().count(el.getID())) {
        m_el->getRelationships().internalAdd(dynamic_cast<DirectedRelationship&>(el));
    }
}

void RemoveDirectedRelationshipFunctor::operator()(Element& el) const {
    if (m_el->getRelationships().count(el.getID())) {
        m_el->getRelationships().internalRemove(dynamic_cast<DirectedRelationship&>(el));
    }
}

void AddOwnedCommentFunctor::operator()(Element& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(dynamic_cast<Comment&>(el));
    }

    if (dynamic_cast<Comment&>(el).getOwningElement() != m_el) {
        dynamic_cast<Comment&>(el).setOwningElement(m_el);
    }
}

void RemoveOwnedCommentFunctor::operator()(Element& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }

    if (dynamic_cast<Comment&>(el).getOwningElement() ==  m_el) {
        dynamic_cast<Comment&>(el).setOwningElement(0);
    }
}

void AddAppliedStereotypeFunctor::operator()(Element& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

void RemoveAppliedStereotypeFunctor::operator()(Element& el) const {
    if (m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalRemove(el);
    }
}

void CheckAppliedStereotypeFunctor::operator()(Element& el) const {
    if (dynamic_cast<InstanceSpecification&>(el).getClassifier() != 0) {
        if (!dynamic_cast<InstanceSpecification&>(el).getClassifier()->isSubClassOf(ElementType::STEREOTYPE)) {
            // TODO: check extension
            throw InvalidAppliedStereotypeException();
        }
    } else {
        throw InvalidAppliedStereotypeException();
    }
}

// Constructor
Element::Element() {
    m_manager = 0;
    m_id = ID::randomID();
    m_ownerPtr = 0;


    m_ownedElements = new Sequence<Element>;
    m_ownedElements->addProcedures.push_back(new SetOwnerFunctor(this));
    m_ownedElements->addChecks.push_back(new ReadOnlySequenceFunctor(this, "ownedElements"));
    m_ownedElements->removeProcedures.push_back(new RemoveOwnerFunctor(this));
    m_ownedElements->removeChecks.push_back(new ReadOnlySequenceFunctor(this, "ownedElements"));
    m_relationships = new Sequence<Relationship>;
    m_relationships->addProcedures.push_back(new AddRelationshipFunctor(this));
    m_relationships->addChecks.push_back(new ReadOnlySequenceFunctor(this, "relationships"));
    m_relationships->removeProcedures.push_back(new RemoveRelationshipFunctor(this));
    m_relationships->removeChecks.push_back(new ReadOnlySequenceFunctor(this, "relationships"));
    m_directedRelationships = new Sequence<DirectedRelationship>;
    m_directedRelationships->addProcedures.push_back(new AddDirectedRelationshipFunctor(this));
    m_directedRelationships->removeProcedures.push_back(new RemoveDirectedRelationshipFunctor(this));
    m_ownedComments = new Sequence<Comment>;
    m_ownedComments->addProcedures.push_back(new AddOwnedCommentFunctor(this));
    m_ownedComments->removeProcedures.push_back(new RemoveOwnedCommentFunctor(this));
    m_appliedStereotype = new Sequence<InstanceSpecification>;
    m_appliedStereotype->addProcedures.push_back(new AddAppliedStereotypeFunctor(this));
    m_appliedStereotype->removeProcedures.push_back(new RemoveAppliedStereotypeFunctor(this));
    m_appliedStereotype->addChecks.push_back(new CheckAppliedStereotypeFunctor(this));
}

// Destructor
Element::~Element() {
    delete m_ownedElements;
    delete m_relationships;
    delete m_directedRelationships;
    delete m_ownedComments;
    delete m_appliedStereotype;
}

Element::Element(const Element& el) {
    m_id = el.m_id;
    m_manager = el.m_manager;

    m_ownerID = el.m_ownerID;
    if (m_manager) {
        m_ownerPtr = 0;
    } else {
        m_ownerPtr = el.m_ownerPtr;
    }

    m_ownedElements = new Sequence<>(*el.m_ownedElements);
    m_relationships = new Sequence<Relationship>(*el.m_relationships);
    m_directedRelationships = new Sequence<DirectedRelationship>(*el.m_directedRelationships);
    m_ownedElements->addProcedures.clear();
    m_ownedElements->addProcedures.push_back(new SetOwnerFunctor(this));
    m_ownedElements->addChecks.clear();
    m_ownedElements->addChecks.push_back(new ReadOnlySequenceFunctor(this, "ownedElements"));
    m_ownedElements->removeProcedures.clear();
    m_ownedElements->removeProcedures.push_back(new RemoveOwnerFunctor(this));
    m_ownedElements->removeChecks.clear();
    m_ownedElements->removeChecks.push_back(new ReadOnlySequenceFunctor(this, "ownedElements"));
    m_relationships->addProcedures.clear();
    m_relationships->addProcedures.push_back(new AddRelationshipFunctor(this));
    m_relationships->addChecks.clear();
    m_relationships->addChecks.push_back(new ReadOnlySequenceFunctor(this, "relationships"));
    m_relationships->removeProcedures.clear();
    m_relationships->removeProcedures.push_back(new RemoveRelationshipFunctor(this));
    m_relationships->removeChecks.clear();
    m_relationships->removeChecks.push_back(new ReadOnlySequenceFunctor(this, "relationships"));
    m_directedRelationships->addProcedures.clear();
    m_directedRelationships->removeProcedures.clear();
    m_directedRelationships->addProcedures.push_back(new AddDirectedRelationshipFunctor(this));
    m_directedRelationships->removeProcedures.push_back(new RemoveDirectedRelationshipFunctor(this));
    m_ownedComments = new Sequence<Comment>(*el.m_ownedComments);
    m_ownedComments->addProcedures.clear();
    m_ownedComments->addProcedures.push_back(new AddOwnedCommentFunctor(this));
    m_ownedComments->removeProcedures.clear();
    m_ownedComments->removeProcedures.push_back(new RemoveOwnedCommentFunctor(this));
    m_appliedStereotype = new Sequence<InstanceSpecification>(*el.m_appliedStereotype);
    m_appliedStereotype->addChecks.clear();
    m_appliedStereotype->addProcedures.clear();
    m_appliedStereotype->removeProcedures.clear();
    m_appliedStereotype->addProcedures.push_back(new AddAppliedStereotypeFunctor(this));
    m_appliedStereotype->removeProcedures.push_back(new RemoveAppliedStereotypeFunctor(this));
    m_appliedStereotype->addChecks.push_back(new CheckAppliedStereotypeFunctor(this));
}

void Element::setID(string id) {
    if (UML::isValidID(id)) {
        setID(ID::fromString(id));
    } else {
        throw InvalidID_Exception();
    }
}

void Element::setID(ID id) {
    reindexID(m_id, id);
    if (m_manager) {
        m_manager->reindex(m_id, id);
    }
    m_id = id;
}

void Element::reindexID(ID oldID, ID newID) {
    if (!m_ownerID.isNull()) {
        if (!m_ownerPtr) {
            m_ownerPtr = &m_manager->get<Element>(m_ownerID);
        }
        m_ownerPtr->m_ownedElements->reindex(oldID, newID);
    }

    for (auto& relationship : *m_relationships) {
        relationship.getRelatedElements().reindex(oldID, newID);
    }
}

// Element* Element::getOwner() {
//     return m_owner;
// }

// void Element::setOwner(Element* owner) {

//     // if owner was already set we need to get rid of previous relationship
//     if (m_owner) {
//         if (m_owner->getOwnedElements().count(m_id)) {
//             m_owner->getOwnedElements().internalRemove(*this);
//         }
//     }

//     // overwrite owner
//     m_owner = owner;

//     // add this to owner's owned elements if not already added and not null
//     if (m_owner) {
//         if (!m_owner->getOwnedElements().count(getID())) {
//             m_owner->getOwnedElements().internalAdd(*this);
//         }
//     }
// }

// Sequence<>& Element::getOwnedElements() {
//     return *m_ownedElements;
// }

Sequence<Relationship>& Element::getRelationships() {
    return *m_relationships;
}

Sequence<DirectedRelationship>& Element::getDirectedRelationships() {
    return *m_directedRelationships;
}

Sequence<Comment>& Element::getOwnedComments() {
    return *m_ownedComments;
}

ElementType Element::getElementType() const {
    return ElementType::ELEMENT;
}

bool Element::isSubClassOf(ElementType eType) const {
    return eType == ElementType::ELEMENT;
}

string Element::elementTypeToString(ElementType eType) {
    switch(eType) {
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
        case ElementType::ASSOCIATION : {
            return "ASSOCIATION";
        }
        case ElementType::BEHAVIOR : {
            return "BEHAVIOR";
        }
        case ElementType::BEHAVIORAL_FEATURE : {
            return "BEHAVIOR_FEATURE";
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
        case ElementType::COMMENT : {
            return "COMMENT";
        }
        case ElementType::CONNECTABLE_ELEMENT : {
            return "CONNECTABLE_ELEMENT";
        }
        case ElementType::CONTROL_FLOW : {
            return "CONTROL_FLOW";
        }
        case ElementType::CREATE_OBJECT_ACTION : {
            return "CREATE_OBJECT_ACTION";
        }
        case ElementType::DATA_TYPE : {
            return "DATA_TYPE";
        }
        case ElementType::DECISION_NODE : {
            return "DECISION_NODE";
        }
        case ElementType::DIRECTED_RELATIONSHIP : {
            return "DIRECTED_RELATIONSHIP";
        }
        case ElementType::ELEMENT : {
            return "ELEMENT";
        }
        case ElementType::ENUMERATION : {
            return "ENUMERATION";
        }
        case ElementType::ENUMERATION_LITERAL : {
            return "ENUMERATION_LITERAL";
        }
        case ElementType::EXPRESSION : {
            return "EXPRESSION";
        }
        case ElementType::EXTENSION : {
            return "EXTENSION";
        }
        case ElementType::FEATURE : {
            return "FEATURE";
        }
        case ElementType::FINAL_NODE : {
            return "FINAL_NODE";
        }
        case ElementType::FORK_NODE : {
            return "FORK_NODE";
        }
        case ElementType::GENERALIZATION : {
            return "GENERALIZATION";
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
        case ElementType::PACKAGE : {
            return "PACKAGE";
        }
        case ElementType::PACKAGEABLE_ELEMENT : {
            return "PACKAGEABLE_ELEMENT";
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
        case ElementType::PARAMETER_NODE : {
            return "PARAMETER_NODE";
        }
        case ElementType::PIN : {
            return "PIN";
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
        case ElementType::REDEFINABLE_ELEMENT : {
            return "REDEFINABLE_ELEMENT";
        }
        case ElementType::RELATIONSHIP : {
            return "RELATIONSHIP";
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
        case ElementType::VALUE_SPECIFICATION : {
            return "VALUE_SPECIFICATION";
        }
        default : {
            return "NOT_SET";
        }
    }
}

string Element::getElementTypeString() const {
    return elementTypeToString(getElementType());
}

void Element::setManager(UmlManager* manager) {
    m_manager = manager;
    m_ownedElements->m_manager = manager;
    m_ownedComments->m_manager = manager;
    m_relationships->m_manager = manager;
    m_directedRelationships->m_manager = manager;
};

ID Element::getID() {
    return m_id;
}

Element* Element::getOwner() {
    if (!m_ownerID.isNull()) {
        if (!m_ownerPtr) {
            if (m_manager) {
                m_ownerPtr = &m_manager->get<Element>(m_ownerID);
            }
        }
        return m_ownerPtr;
    }
    return 0;
}

void Element::setOwner(Element* owner) {
    // remove old owner
    if (!isSameOrNull(m_ownerID, owner)) {
        if (!m_ownerPtr) {
            *m_ownerPtr = m_manager->get<>(m_ownerID);
        }
        if (m_ownerPtr->getOwnedElements().count(m_id)) {
            m_ownerPtr->getOwnedElements().internalRemove(*this);
        }
        m_ownerID = ID::nullID();
        m_ownerPtr = 0;
    }

    // set owner id
    if (owner) {
        m_ownerID = owner->getID();
    }

    // set owner ptr
    if (!m_manager) {
        m_ownerPtr = owner;
    } else {
        m_ownerPtr = 0;
    }

    // add to owner owned elements 
    if (owner) {
        if (!owner->getOwnedElements().count(m_id)) {
            owner->getOwnedElements().internalAdd(*this);
        }
    }
}

Sequence<Element>& Element::getOwnedElements() {
    return *m_ownedElements;
}

Sequence<InstanceSpecification>& Element::getAppliedStereotypes() {
    return *m_appliedStereotype;
}

/**
 * This func compares an id and an element without loading 
 * the element that corresponds to the id from the manager
 **/
bool Element::isSameOrNull(ID id, Element* el) {
    if (id.isNull()) {
        return true;
    } else {
        if (el) {
            return id == el->getID();
        }
        return false;
    }
}
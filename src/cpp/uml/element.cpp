#include "uml/element.h"
#include "uml/umlManager.h"
#include "uml/sequence.h"
#include "uml/relationship.h"
#include "uml/elementFunctors.h"
#include "uml/directedRelationship.h"
#include "uml/comment.h"
#include "uml/instanceSpecification.h"
#include "uml/classifier.h"
#include "uml/universalFunctions.h"

using namespace std;
using namespace UML;

void SetOwnerFunctor::operator()(Element& el) const{
    oppositeSingletonAdd(el, &Element::setOwner);
    updateCopiedSequenceAddedTo(el, &Element::getOwnedElements);
}

void AddRelationshipFunctor::operator()(Relationship& el) const {
    oppositeSequenceAdd(el, &Relationship::getRelatedElements);
    updateCopiedSequenceAddedTo(el, &Element::getRelationships);
}

void RemoveRelationshipFunctor::operator()(Relationship& el) const {
    oppositeSequenceRemove(el, &Relationship::getRelatedElements);
    updateCopiedSequenceRemovedFrom(el, &Element::getRelationships);
}

void RemoveOwnerFunctor::operator()(Element& el) const {
    oppositeSingletonRemove(el, &Element::m_ownerID, &Element::setOwner);
    updateCopiedSequenceRemovedFrom(el, &Element::getOwnedElements);
}

void AddDirectedRelationshipFunctor::operator()(DirectedRelationship& el) const {
    subsetsAdd<Element, Relationship>(el, &Element::getRelationships);
    updateCopiedSequenceAddedTo(el, &Element::getDirectedRelationships);
}

void RemoveDirectedRelationshipFunctor::operator()(DirectedRelationship& el) const {
    subsetsRemove<Element, Relationship>(el, &Element::getRelationships);
    updateCopiedSequenceRemovedFrom(el, &Element::getDirectedRelationships);
}

void AddOwnedCommentFunctor::operator()(Comment& el) const {
    subsetsAdd<Element, Element>(el, &Element::getOwnedElements);
    oppositeSingletonAdd(el, &Comment::setOwningElement);
    updateCopiedSequenceAddedTo(el, &Element::getOwnedComments);
}

void RemoveOwnedCommentFunctor::operator()(Comment& el) const {
    subsetsRemove<Element, Element>(el, &Element::getOwnedElements);
    oppositeSingletonRemove(el, &Comment::m_owningElement);
    updateCopiedSequenceRemovedFrom(el, &Element::getOwnedComments);
}

void AddAppliedStereotypeFunctor::operator()(InstanceSpecification& el) const {
    subsetsAdd<Element,Element>(el, &Element::getOwnedElements);
    updateCopiedSequenceAddedTo(el, &Element::getAppliedStereotypes);
}

void RemoveAppliedStereotypeFunctor::operator()(InstanceSpecification& el) const {
    subsetsRemove<Element, Element>(el, &Element::getOwnedElements);
    updateCopiedSequenceRemovedFrom(el, &Element::getAppliedStereotypes);
}

void CheckAppliedStereotypeFunctor::operator()(InstanceSpecification& el) const {
    if (el.getClassifier() != 0) {
        if (!el.getClassifier()->isSubClassOf(ElementType::STEREOTYPE)) {
            // TODO: check extension
            throw InvalidAppliedStereotypeException();
        }
    } else {
        throw InvalidAppliedStereotypeException();
    }
}

void Element::setReference(Element* referencing) {
    if (this == m_node->m_managerElementMemory) {
        if (m_node->m_references.count(referencing->getID())) {
            m_node->m_referenceCount[referencing->getID()]++;
        } else {
            m_node->m_references[referencing->getID()] = referencing->m_node;
            m_node->m_referenceCount[referencing->getID()] = 1;
            m_node->m_referenceOrder.push_back(referencing->getID());
        }
    }
}

void Element::removeReference(ID referencing) {
    if (this == m_node->m_managerElementMemory) {
        if (m_node->m_referenceCount[referencing] > 1) {
            m_node->m_referenceCount[referencing]--;
        } else {
            m_node->m_references.erase(referencing);
            m_node->m_referenceCount.erase(referencing);
            m_node->m_referenceOrder.erase(std::remove(
                m_node->m_referenceOrder.begin(), 
                m_node->m_referenceOrder.end(), 
                referencing), m_node->m_referenceOrder.end());
        }
    }
}

// Constructor
Element::Element() {
    m_manager = 0;
    m_node = 0;
    m_id = ID::randomID();
    m_ownerPtr = 0;


    m_ownedElements = new Sequence<Element>(this);
    m_ownedElements->addProcedures.push_back(new SetOwnerFunctor(this));
    m_ownedElements->addChecks.push_back(new ReadOnlySequenceFunctor(this, "ownedElements"));
    m_ownedElements->removeProcedures.push_back(new RemoveOwnerFunctor(this));
    m_ownedElements->removeChecks.push_back(new ReadOnlySequenceFunctor(this, "ownedElements"));
    m_relationships = new Sequence<Relationship>(this);
    m_relationships->addProcedures.push_back(new AddRelationshipFunctor(this));
    m_relationships->addChecks.push_back(new ReadOnlySequenceFunctor<Relationship>(this, "relationships"));
    m_relationships->removeProcedures.push_back(new RemoveRelationshipFunctor(this));
    m_relationships->removeChecks.push_back(new ReadOnlySequenceFunctor<Relationship>(this, "relationships"));
    m_directedRelationships = new Sequence<DirectedRelationship>(this);
    m_directedRelationships->addProcedures.push_back(new AddDirectedRelationshipFunctor(this));
    m_directedRelationships->removeProcedures.push_back(new RemoveDirectedRelationshipFunctor(this));
    m_ownedComments = new Sequence<Comment>(this);
    m_ownedComments->addProcedures.push_back(new AddOwnedCommentFunctor(this));
    m_ownedComments->removeProcedures.push_back(new RemoveOwnedCommentFunctor(this));
    m_appliedStereotype = new Sequence<InstanceSpecification>(this);
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
    if (m_manager) {
        if (m_node->m_copies.count(this)) {
            m_node->m_copies.erase(this);
        }
    }
}

Element::Element(const Element& el) {
    m_id = el.m_id;
    m_manager = el.m_manager;
    m_node = el.m_node;

    m_ownerID = el.m_ownerID;
    if (m_manager) {
        m_manager->m_graph[m_id].m_copies.insert(this);
        m_ownerPtr = 0;
    } else {
        m_ownerPtr = el.m_ownerPtr;
    }

    m_ownedElements = new Sequence<>(*el.m_ownedElements);
    m_ownedElements->m_el = this;
    m_relationships = new Sequence<Relationship>(*el.m_relationships);
    m_relationships->m_el = this;
    m_directedRelationships = new Sequence<DirectedRelationship>(*el.m_directedRelationships);
    m_directedRelationships->m_el = this;
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
    m_relationships->addChecks.push_back(new ReadOnlySequenceFunctor<Relationship>(this, "relationships"));
    m_relationships->removeProcedures.clear();
    m_relationships->removeProcedures.push_back(new RemoveRelationshipFunctor(this));
    m_relationships->removeChecks.clear();
    m_relationships->removeChecks.push_back(new ReadOnlySequenceFunctor<Relationship>(this, "relationships"));
    m_directedRelationships->addProcedures.clear();
    m_directedRelationships->removeProcedures.clear();
    m_directedRelationships->addProcedures.push_back(new AddDirectedRelationshipFunctor(this));
    m_directedRelationships->removeProcedures.push_back(new RemoveDirectedRelationshipFunctor(this));
    m_ownedComments = new Sequence<Comment>(*el.m_ownedComments);
    m_ownedComments->m_el = this;
    m_ownedComments->addProcedures.clear();
    m_ownedComments->addProcedures.push_back(new AddOwnedCommentFunctor(this));
    m_ownedComments->removeProcedures.clear();
    m_ownedComments->removeProcedures.push_back(new RemoveOwnedCommentFunctor(this));
    m_appliedStereotype = new Sequence<InstanceSpecification>(*el.m_appliedStereotype);
    m_appliedStereotype->m_el = this;
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
        case ElementType::ABSTRACTION : {
            return "ABSTRACTION";
        }
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
        case ElementType::REALIZATION : {
            return "REALIZATION";
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
        case ElementType::USAGE : {
            return "USAGE";
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
    m_appliedStereotype->m_manager = manager;
};

ID Element::getID() {
    return m_id;
}

Element* Element::getOwner() {
    if (m_manager) {
        return m_manager->get<>(this, m_ownerID, &Element::m_ownerPtr);
    } else {
        return m_ownerPtr;
    }
}

void Element::setOwner(Element* owner) {
    if (!isSameOrNull(m_ownerID, owner)) {
        if (!m_ownerPtr) {
            m_ownerPtr = m_manager->get<>(this, m_ownerID);
        }
        if (m_manager) {
            removeReference(m_ownerID);
        }
        m_ownerID = ID::nullID();
        if (m_ownerPtr->getOwnedElements().count(m_id)) {
            m_ownerPtr->getOwnedElements().internalRemove(*this);
        }
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
        if (m_manager) {
            setReference(owner);
            // if the owner is mounted we need to mount
            if (!m_manager->m_graph[m_ownerID].m_mountPath.empty()) {
                m_manager->setElementAndChildrenMount(filesystem::path(m_manager->m_graph[m_ownerID].m_mountPath).parent_path(), 
                                                      *m_manager->m_graph[m_id].m_managerElementMemory);
            }
        }
        if (!owner->getOwnedElements().count(m_id)) {
            owner->getOwnedElements().internalAdd(*this);
        }
    }
    if (m_manager) {
        if (m_node) {
            if (m_node->m_managerElementMemory != this) {
                if (m_node->m_managerElementMemory->m_ownerID != m_ownerID) {
                    m_node->m_managerElementMemory->m_ownerID = m_ownerID;
                    m_node->m_managerElementMemory->m_ownerPtr = 0;
                }
            }
            for (auto& copy : m_node->m_copies) {
                if (copy != this) {
                    if (copy->m_ownerID != m_ownerID) {
                        copy->m_ownerID = m_ownerID;
                        copy->m_ownerPtr = 0;
                    }
                }
            }
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

void Element::restoreReleased(ID id, Element* el) {
    if (m_ownedElements->count(id)) {
        el->setOwner(this);
    }
}

void Element::referencingReleased(ID id) {
    if (m_ownerID == id) {
        m_ownerPtr = 0;
    }

    m_ownedElements->elementReleased(id);
}
#include "uml/element.h"
#include "uml/umlManager.h"
// #include "uml/sequence.h"
// #include "uml/relationship.h"
// #include "uml/directedRelationship.h"
// #include "uml/comment.h"
// #include "uml/instanceSpecification.h"
// #include "uml/classifier.h"
#include "uml/singleton.h"

using namespace std;
namespace UML {

class AddToMountFunctor : public SetFunctor {
    public:
        AddToMountFunctor(Element* them) : SetFunctor(them) {};
        void operator()(Element& el) const override {
            if (!m_el.m_manager->m_mountBase.empty() && m_el.m_manager->count(el.getID())) {
                m_el.m_manager->addToMount(m_el);
            }
        };
};

void Element::setReference(Element* referencing) {
    if (m_node->m_references.count(referencing->getID())) {
        m_node->m_referenceCount[referencing->getID()]++;
    } else {
        m_node->m_references[referencing->getID()] = referencing->m_node;
        m_node->m_referenceCount[referencing->getID()] = 1;
        m_node->m_referenceOrder.push_back(referencing->getID());
    }
}

void Element::setReference(ID id) {
    if (m_node->m_references.count(id)) {
        m_node->m_referenceCount[id]++;
    } else {
        m_node->m_references[id] = 0;
        m_node->m_referenceCount[id] = 1;
        m_node->m_referenceOrder.push_back(id);
    }
}

void Element::removeReference(ID referencing) {
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

void Element::referenceReindexed(ID oldID, ID newID) {
    m_ownedElements->reindex(oldID, newID);
    m_owner->reindex(oldID, newID);
    // m_ownedComments->reindex(oldID, newID);
    // m_relationships->reindex(oldID, newID);
    // m_directedRelationships->reindex(oldID, newID);
    // m_appliedStereotype->reindex(oldID, newID);
}

void Element::referencingReleased(ID id) {
    if (m_node->m_references.count(id)) {
        m_node->m_references[id] = 0;
    }
    m_owner->release(id);
    m_ownedElements->release(id);
    // m_relationships->release(id);
    // m_directedRelationships->release(id);
    // m_ownedComments->release(id);
    // m_appliedStereotype->release(id);
}

void Element::restoreReferences() {
    //m_ownedElements->restoreReferences();
    // if (hasOwner()) {
    //     if (m_manager->loaded(m_ownerID)) {
    //         m_manager->get(this, m_ownerID)->restoreReference(this);
    //     }
    // }
    // m_ownedComments->restoreReferences();
    // m_relationships->restoreReferences();
    // m_directedRelationships->restoreReferences();
    // m_appliedStereotype->restoreReferences();
}

void Element::restoreReference(Element* el) {
    if (m_node->m_references.count(el->getID())) {
        m_node->m_references[el->getID()] = el->m_node;
    }
}

void Element::referenceErased(ID id) {
    m_owner->eraseElement(id);
    m_ownedElements->eraseElement(id);
    // m_ownedComments->eraseElement(id);
    // m_relationships->eraseElement(id);
    // m_directedRelationships->eraseElement(id);
    // m_appliedStereotype->eraseElement(id);
}

Set<Element, Element>& Element::getOwnerSingleton() {
    return *m_owner;
}

Element::Element(ElementType elementType) : m_elementType(elementType) {
    m_manager = 0;
    m_node = 0;
    m_id = ID::randomID();

    m_owner = new Singleton<Element, Element>(this);
    m_owner->m_signature = &Element::getOwnerSingleton;
    m_owner->m_readOnly = true;
    m_owner->m_addFunctors.insert(new AddToMountFunctor(this));

    m_ownedElements = new Set<Element, Element>(this);
    m_ownedElements->opposite(&Element::getOwnerSingleton);
    m_ownedElements->m_signature = &Element::getOwnedElements;
    m_ownedElements->m_readOnly = true;

    // m_relationships = new Set<Relationship, Element>(this);

    // m_directedRelationships = new Set<DirectedRelationship>(this);
    // m_directedRelationships->subsets(*m_relationships);

    // m_ownedComments = new Set<Comment>(this);
    // m_ownedComments->subsets(*m_ownedElements);

    // m_appliedStereotype = new Set<InstanceSpecification>(this);
    // m_appliedStereotype->subsets(*m_ownedElements);
}

Element::~Element() {
    delete m_owner;
    delete m_ownedElements;
    // delete m_relationships;
    // delete m_directedRelationships;
    // delete m_ownedComments;
    // delete m_appliedStereotype;
    if (m_copiedElementFlag) {
        if (m_manager) {
            if (m_node->m_copies.count(this)) {
                m_node->m_copies.erase(this);
            }
        }
    }
}

Element::Element(const Element& el, ElementType elementType) : Element(elementType) {
    m_copiedElementFlag = true;
    m_id = el.m_id;
    m_manager = el.m_manager;
    m_node = el.m_node;
    if (m_manager) {
        m_manager->m_graph[m_id].m_copies.insert(this);
    }

    *m_owner = *el.m_owner;
    m_owner->m_el = this;
    // m_owner->m_addFunctors.insert(new AddToMountFunctor(this));
    *m_ownedElements = *el.m_ownedElements;
    m_ownedElements->m_el = this;
    // m_relationships = new Set<Relationship, Element>(*el.m_relationships);
    // m_relationships->m_el = this;
    // m_directedRelationships = new Set<DirectedRelationship, Element>(*el.m_directedRelationships);
    // m_directedRelationships->m_el = this;
    // m_ownedComments = new Set<Comment, Element>(*el.m_ownedComments);
    // m_ownedComments->m_el = this;
    // m_appliedStereotype = new Set<InstanceSpecification, Element>(*el.m_appliedStereotype);
    // m_appliedStereotype->m_el = this;
}

void Element::setID(string id) {
    if (UML::isValidID(id)) {
        setID(ID::fromString(id));
    } else {
        throw InvalidID_Exception();
    }
}

void Element::setID(ID id) {
    if (m_manager) {
        m_manager->reindex(m_id, id);
    }
    m_id = id;
    if (m_manager) {
        for (auto& copy : m_node->m_copies) {
            if (copy != this) {
                copy->m_id = id;
            }
        }
        if (m_node->m_managerElementMemory != this) {
            m_node->m_managerElementMemory->m_id = id;
        }
    }
}

// Set<Relationship, Element>& Element::getRelationships() {
//     return *m_relationships;
// }

// Set<DirectedRelationship, Element>& Element::getDirectedRelationships() {
//     return *m_directedRelationships;
// }

ElementType Element::getElementType() const {
    return m_elementType;
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

ID Element::getID() const {
    return m_id;
}

Element* Element::getOwner() {
    return m_owner->get();
}

Element& Element::getOwnerRef() {
    return m_owner->getRef();
}

ID Element::getOwnerID() const {
    return m_owner->id();
}

bool Element::hasOwner() const {
    return m_owner->has();
}

void Element::setOwner(Element* owner) {
    m_owner->set(owner);
}

void Element::setOwnerByID(ID id) {
   m_owner->set(id);
}

Set<Element, Element>& Element::getOwnedElements() {
    return *m_ownedElements;
}

// Set<InstanceSpecification, Element>& Element::getAppliedStereotypes() {
//     return *m_appliedStereotype;
// }

// Set<Comment, Element>& Element::getOwnedComments() {
//     return *m_ownedComments;
// }

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
}
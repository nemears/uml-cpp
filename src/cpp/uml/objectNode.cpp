#include "uml/objectNode.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"
#include "uml/setReferenceFunctor.h"

using namespace UML;

Set<ValueSpecification, ObjectNode>& ObjectNode::getUpperBoundSingleton() {
    return m_upperBound;
}

Set<Behavior, ObjectNode>& ObjectNode::getSelectionSingleton() {
    return m_selection;
}

void ObjectNode::referencingReleased(ID id) {
    ActivityNode::referencingReleased(id);
    TypedElement::referencingReleased(id);
    m_selection.release(id);
}

void ObjectNode::referenceReindexed(ID oldID, ID newID) {
    ActivityNode::referenceReindexed(oldID, newID);
    TypedElement::referenceReindexed(oldID, newID);
    m_selection.reindex(oldID, newID);
}

void ObjectNode::reindexName(ID id, std::string newName) {
    ActivityNode::reindexName(id, newName);
    TypedElement::reindexName(id, newName);
    m_selection.reindexName(id, newName);
}

void ObjectNode::referenceErased(ID id) {
    ActivityNode::referenceErased(id);
    TypedElement::referenceErased(id);
    m_selection.eraseElement(id);
}

void ObjectNode::init() {
    m_upperBound.subsets(*m_ownedElements);
    m_upperBound.m_signature = &ObjectNode::getUpperBoundSingleton;
    m_selection.m_signature = &ObjectNode::getSelectionSingleton;
    m_selection.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_selection.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
}

ObjectNode::ObjectNode() : Element(ElementType::OBJECT_NODE) {
    init();
}

ObjectNode::~ObjectNode() {
    
}

bool ObjectNode::isControlType() const {
    return m_controlType;
}

void ObjectNode::setControlType(bool controlType) {
    m_controlType = controlType;
}

ObjectNodeOrderingKind ObjectNode::getOrdering() const {
    return m_ordering;
}

void ObjectNode::setOrdering(ObjectNodeOrderingKind ordering) {
    m_ordering = ordering;
}

ValueSpecificationPtr ObjectNode::getUpperBound() const {
    return m_upperBound.get();
}

void ObjectNode::setUpperBound(ValueSpecification* upperBound) {
    m_upperBound.set(upperBound);
}

void ObjectNode::setUpperBound(ValueSpecification& upperBound) {
    m_upperBound.set(upperBound);
}

void ObjectNode::setUpperBound(ValueSpecificationPtr upperBound) {
    m_upperBound.set(upperBound);
}

void ObjectNode::setUpperBound(ID id) {
    m_upperBound.set(id);
}

BehaviorPtr ObjectNode::getSelection() const {
    return m_selection.get();
}

void ObjectNode::setSelection(Behavior* selection) {
    m_selection.set(selection);
}

void ObjectNode::setSelection(Behavior& selection) {
    m_selection.set(selection);
}

void ObjectNode::setSelection(BehaviorPtr selection) {
    m_selection.set(selection);
}

void ObjectNode::setSelection(ID id) {
    m_selection.set(id);
}

bool ObjectNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = TypedElement::isSubClassOf(eType);
    }

    if (!ret) {
        ret = eType == ElementType::OBJECT_NODE;
    }

    return ret;
}
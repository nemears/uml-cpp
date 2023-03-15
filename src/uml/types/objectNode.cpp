#include "uml/types/objectNode.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<ValueSpecification, ObjectNode>& ObjectNode::getUpperBoundSingleton() {
    return m_upperBound;
}

TypedSet<Behavior, ObjectNode>& ObjectNode::getSelectionSingleton() {
    return m_selection;
}

void ObjectNode::referenceReindexed(ID newID) {
    ActivityNode::referenceReindexed(newID);
    TypedElement::referenceReindexed(newID);
    m_selection.reindex(newID);
}

void ObjectNode::referenceErased(ID id) {
    ActivityNode::referenceErased(id);
    TypedElement::referenceErased(id);
    m_selection.eraseElement(id);
}

ObjectNode::ObjectNode() : Element(ElementType::OBJECT_NODE) {
    m_upperBound.subsets(*m_ownedElements);
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
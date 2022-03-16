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

using namespace UML;

Set<ValueSpecification, ObjectNode>& ObjectNode::getUpperBoundSingleton() {
    return m_upperBound;
}

void ObjectNode::referencingReleased(ID id) {
    ActivityNode::referencingReleased(id);
    TypedElement::referencingReleased(id);
}

void ObjectNode::referenceReindexed(ID oldID, ID newID) {
    ActivityNode::referenceReindexed(oldID, newID);
    TypedElement::referenceReindexed(oldID, newID);
}

void ObjectNode::reindexName(ID id, std::string newName) {
    ActivityNode::reindexName(id, newName);
    TypedElement::reindexName(id, newName);
}

void ObjectNode::init() {
    m_upperBound.subsets(*m_ownedElements);
    m_upperBound.m_signature = &ObjectNode::getUpperBoundSingleton;
}

ObjectNode::ObjectNode() : Element(ElementType::OBJECT_NODE) {
    init();
}

ObjectNode::~ObjectNode() {
    
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

void ObjectNode::setUpperBound(ID id) {
    m_upperBound.set(id);
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
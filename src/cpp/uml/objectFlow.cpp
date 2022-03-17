#include "uml/objectFlow.h"
#include "uml/activityNode.h"
#include "uml/activity.h"
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

Set<Behavior, ObjectFlow>& ObjectFlow::getTransformationSingleton() {
    return m_transformation;
}

Set<Behavior, ObjectFlow>& ObjectFlow::getSelectionSingleton() {
    return m_selection;
}

void ObjectFlow::referencingReleased(ID id) {
    ActivityEdge::referencingReleased(id);
    m_transformation.release(id);
    m_selection.release(id);
}

void ObjectFlow::referenceReindexed(ID oldID, ID newID) {
    ActivityEdge::referenceReindexed(oldID, newID);
    m_transformation.reindex(oldID, newID);
    m_selection.reindex(oldID, newID);
}

void ObjectFlow::referenceErased(ID id) {
    ActivityEdge::referenceErased(id);
    m_transformation.eraseElement(id);
    m_selection.eraseElement(id);
}

void ObjectFlow::restoreReference(Element* el) {
    NamedElement::restoreReference(el);
}

void ObjectFlow::init() {
    m_transformation.m_signature = &ObjectFlow::getTransformationSingleton;
    m_selection.m_signature = &ObjectFlow::getSelectionSingleton;
}

ObjectFlow::ObjectFlow() : Element(ElementType::OBJECT_FLOW) {

}

ObjectFlow::~ObjectFlow() {

}

BehaviorPtr ObjectFlow::getTransformation() const {
    return m_transformation.get();
}

void ObjectFlow::setTransformation(Behavior* transformation) {
    m_transformation.set(transformation);
}

void ObjectFlow::setTransformation(Behavior& transformation) {
    m_transformation.set(transformation);
}

void ObjectFlow::setTransformation(ID id) {
    m_transformation.set(id);
}

void ObjectFlow::setTransformation(BehaviorPtr transformation) {
    m_transformation.set(transformation);
}

BehaviorPtr ObjectFlow::getSelection() const {
    return m_selection.get();
}

void ObjectFlow::setSelection(Behavior* selection) {
    m_selection.set(selection);
}

void ObjectFlow::setSelection(Behavior& selection) {
    m_selection.set(selection);
}

void ObjectFlow::setSelection(BehaviorPtr selection) {
    m_selection.set(selection);
}

void ObjectFlow::setSelection(ID id) {
    m_selection.set(id);
}

bool ObjectFlow::isSubClassOf(ElementType eType) const {
    bool ret = ActivityEdge::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::OBJECT_FLOW;
    }

    return ret;
}
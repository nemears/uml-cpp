#include "uml/types/objectFlow.h"
#include "uml/types/activityNode.h"
#include "uml/types/activity.h"
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

TypedSet<Behavior, ObjectFlow>& ObjectFlow::getTransformationSingleton() {
    return m_transformation;
}

TypedSet<Behavior, ObjectFlow>& ObjectFlow::getSelectionSingleton() {
    return m_selection;
}

void ObjectFlow::referenceReindexed(ID newID) {
    ActivityEdge::referenceReindexed(newID);
    m_transformation.reindex(newID);
    m_selection.reindex(newID);
}

void ObjectFlow::referenceErased(ID id) {
    ActivityEdge::referenceErased(id);
    m_transformation.eraseElement(id);
    m_selection.eraseElement(id);
}

void ObjectFlow::restoreReference(Element* el) {
    NamedElement::restoreReference(el);
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
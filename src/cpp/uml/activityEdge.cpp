#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace std;
using namespace UML;

void ActivityEdge::RemoveActivityProcedure::operator()(Activity* el) const {
    if (el->getEdges().count(m_me->getID())) {
        el->getEdges().remove(*m_me);
    }
}

void ActivityEdge::AddActivityProcedure::operator()(Activity* el) const {
    if (!el->getEdges().count(m_me->getID())) {
        el->getEdges().add(*m_me);
    }
}

void ActivityEdge::RemoveSourceProcedure::operator()(ActivityNode* el) const {
    if (el->getOutgoing().count(m_me->getID())) {
        el->getOutgoing().remove(*m_me);
    }
}

void ActivityEdge::AddSourceProcedure::operator()(ActivityNode* el) const {
    if (!el->getOutgoing().count(m_me->getID())) {
        el->getOutgoing().add(*m_me);
    }
}

void ActivityEdge::RemoveTargetProcedure::operator()(ActivityNode* el) const {
    if (el->getIncoming().count(m_me->getID())) {
        el->getIncoming().remove(*m_me);
    }
}

void ActivityEdge::AddTargetProcedure::operator()(ActivityNode* el) const {
    if (!el->getIncoming().count(m_me->getID())) {
        el->getIncoming().add(*m_me);
    }
}

void ActivityEdge::RemoveGuardProcedure::operator()(ValueSpecification* el) const {
    if (m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalRemove(*el);
    }
}

void ActivityEdge::AddGuardProcedure::operator()(ValueSpecification* el) const {
    if (!m_me->getOwnedElements().count(el->getID())) {
        m_me->getOwnedElements().internalAdd(*el);
    }
}

ActivityEdge::ActivityEdge() {
    m_activity.m_signature = &ActivityEdge::m_activity;
    m_activity.m_removeProcedures.push_back(new RemoveActivityProcedure(this));
    m_activity.m_addProcedures.push_back(new AddActivityProcedure(this));
    m_source.m_signature = &ActivityEdge::m_source;
    m_source.m_removeProcedures.push_back(new RemoveSourceProcedure(this));
    m_source.m_addProcedures.push_back(new AddSourceProcedure(this));
    m_target.m_signature = &ActivityEdge::m_target;
    m_target.m_removeProcedures.push_back(new RemoveTargetProcedure(this));
    m_target.m_addProcedures.push_back(new AddTargetProcedure(this));
    m_guard.m_signature = &ActivityEdge::m_guard;
    m_guard.m_removeProcedures.push_back(new RemoveGuardProcedure(this));
    m_guard.m_addProcedures.push_back(new AddGuardProcedure(this));
}

ActivityEdge::ActivityEdge(const ActivityEdge& rhs) : RedefinableElement(rhs) , NamedElement(rhs), Element(rhs) {
    m_activity = rhs.m_activity;
    m_activity.m_me = this;
    m_activity.m_addProcedures.clear();
    m_activity.m_removeProcedures.clear();
    m_activity.m_removeProcedures.push_back(new RemoveActivityProcedure(this));
    m_activity.m_addProcedures.push_back(new AddActivityProcedure(this));
    m_source = rhs.m_source;
    m_source.m_me = this;
    m_source.m_removeProcedures.clear();
    m_source.m_addProcedures.clear();
    m_source.m_removeProcedures.push_back(new RemoveSourceProcedure(this));
    m_source.m_addProcedures.push_back(new AddSourceProcedure(this));
    m_target = rhs.m_target;
    m_target.m_me = this;
    m_target.m_removeProcedures.clear();
    m_target.m_addProcedures.clear();
    m_target.m_removeProcedures.push_back(new RemoveTargetProcedure(this));
    m_target.m_addProcedures.push_back(new AddTargetProcedure(this));
    m_guard = rhs.m_guard;
    m_guard.m_me = this;
    m_guard.m_removeProcedures.clear();
    m_guard.m_addProcedures.clear();
    m_guard.m_removeProcedures.push_back(new RemoveGuardProcedure(this));
    m_guard.m_addProcedures.push_back(new AddGuardProcedure(this));
}

ActivityEdge::~ActivityEdge() {

}

// void ActivityEdge::reindexName(string oldName, string newName) {
//     if (m_target) {
//         m_target->getIncoming().reindex(m_id, oldName, newName);
//     }

//     if (m_source) {
//         m_source->getOutgoing().reindex(m_id, oldName, newName);
//     }

//     if (m_activity) {
//         m_activity->getEdges().reindex(m_id, oldName, newName);
//     }

//     NamedElement::reindexName(oldName, newName);
// }

Activity* ActivityEdge::getActivity() {
    return m_activity.get();
}

Activity& ActivityEdge::getActivityRef() {
    return m_activity.getRef();
}

bool ActivityEdge::hasActivity() const {
    return m_activity.has();
}

void ActivityEdge::setActivity(Activity* activity) {
    m_activity.set(activity);
}

void ActivityEdge::setActivity(Activity& activity) {
    m_activity.set(activity);
}

ActivityNode* ActivityEdge::getSource() {
    return m_source.get();
}

ActivityNode& ActivityEdge::getSourceRef() {
    return m_source.getRef();
}

bool ActivityEdge::hasSource() const {
    return m_source.has();
}

void ActivityEdge::setSource(ActivityNode* source) {
    m_source.set(source);
}

void ActivityEdge::setSource(ActivityNode& source) {
    m_source.set(source);
}

ActivityNode* ActivityEdge::getTarget() {
    return m_target.get();
}

ActivityNode& ActivityEdge::getTargetRef() {
    return m_target.getRef();
}

bool ActivityEdge::hasTarget() const {
    return m_target.has();
}

void ActivityEdge::setTarget(ActivityNode* target) {
    m_target.set(target);
}

void ActivityEdge::setTarget(ActivityNode& target) {
    m_target.set(target);
}

ValueSpecification* ActivityEdge::getGuard() {
    return m_guard.get();
}

ValueSpecification& ActivityEdge::getGuardRef() {
    return m_guard.getRef();
}

bool ActivityEdge::hasGuard() const {
    return m_guard.has();
}

void ActivityEdge::setGuard(ValueSpecification* guard) {
    m_guard.set(guard);
}

void ActivityEdge::setGuard(ValueSpecification& guard) {
    m_guard.set(guard);
}

ElementType ActivityEdge::getElementType() const {
    return ElementType::ACTIVITY_EDGE;
}

bool ActivityEdge::isSubClassOf(ElementType eType) const {
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_EDGE;
    }

    return ret;
}

void ActivityEdge::referencingReleased(ID id) {
    if (m_activity.id() == id) {
        m_activity.release();
    }
    if (m_source.id() == id) {
        m_source.release();
    }
    if (m_target.id() == id) {
        m_target.release();
    }
    if (m_guard.id() == id) {
        m_guard.release();
    }
}
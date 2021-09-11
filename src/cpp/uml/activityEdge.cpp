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

ActivityEdge::ActivityEdge() {
    m_activity.m_signature = &ActivityEdge::m_activity;
    m_activity.m_removeProcedures.push_back(new RemoveActivityProcedure(this));
    m_activity.m_addProcedures.push_back(new AddActivityProcedure(this));
    m_source = 0;
    m_target = 0;
    m_guard = 0;
}

ActivityEdge::ActivityEdge(const ActivityEdge& rhs) : RedefinableElement(rhs) , NamedElement(rhs), Element(rhs) {
    m_activity = rhs.m_activity;
    m_activity.m_me = this;
    m_activity.m_addProcedures.clear();
    m_activity.m_removeProcedures.clear();
    m_activity.m_removeProcedures.push_back(new RemoveActivityProcedure(this));
    m_activity.m_addProcedures.push_back(new AddActivityProcedure(this));
}

ActivityEdge::~ActivityEdge() {

}

void ActivityEdge::reindexID(ID oldID, ID newID) {
    if (m_target) {
        m_target->getIncoming().reindex(oldID, newID);
    }

    if (m_source) {
        m_source->getOutgoing().reindex(oldID, newID);
    }
    if (m_activity.has()) {
        m_activity.getRef().getEdges().reindex(oldID, newID);
    }
    RedefinableElement::reindexID(oldID, newID);
    NamedElement::reindexID(oldID, newID);
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
    return m_source;
}

void ActivityEdge::setSource(ActivityNode* source) {
    if (m_source) {
        if (dynamic_cast<ActivityNode*>(m_source)->getOutgoing().count(m_id)) {
            dynamic_cast<ActivityNode*>(m_source)->getOutgoing().remove(*this);
        }
    }
    m_source = source;
    if (!m_source->getOutgoing().count(m_id)) {
        m_source->getOutgoing().add(*this);
    }
}

ActivityNode* ActivityEdge::getTarget() {
    return m_target;
}

void ActivityEdge::setTarget(ActivityNode* target) {
    if (m_target) {
        if (dynamic_cast<ActivityNode*>(m_target)->getIncoming().count(m_id)) {
            dynamic_cast<ActivityNode*>(m_target)->getIncoming().remove(*this);
        }
    }
    m_target = target;
    if (!m_target->getIncoming().count(m_id)) {
        m_target->getIncoming().add(*this);
    }
}

ValueSpecification* ActivityEdge::getGuard() {
    return m_guard;
}

void ActivityEdge::setGuard(ValueSpecification* guard) {
    m_guard = guard;
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

void ActivityEdge::restoreReleased(ID id, Element* released) {
    RedefinableElement::restoreReleased(id, released);
    /** TODO: do anythin? **/
}

void ActivityEdge::referencingReleased(ID id) {
    if (m_activity.id() == id) {
        m_activity.release();
    }
}
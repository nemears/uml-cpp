#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace std;
using namespace UML;

void ActivityEdge::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_target) {
        m_target->getIncoming().reindex(oldID, newID);
    }

    if (m_source) {
        m_source->getOutgoing().reindex(oldID, newID);
    }

    if (m_activity) {
        m_activity->getEdges().reindex(oldID, newID);
    }

    NamedElement::reindexID(oldID, newID);
}

void ActivityEdge::reindexName(string oldName, string newName) {
    if (m_target) {
        m_target->getIncoming().reindex(m_id, oldName, newName);
    }

    if (m_source) {
        m_source->getOutgoing().reindex(m_id, oldName, newName);
    }

    if (m_activity) {
        m_activity->getEdges().reindex(m_id, oldName, newName);
    }

    NamedElement::reindexName(oldName, newName);
}

Activity* ActivityEdge::getActivity() {
    return m_activity;
}

void ActivityEdge::setActivity(Activity* activity) {
    m_activity = activity;
    if(!m_activity->getEdges().count(m_id)) {
        m_activity->getEdges().add(*this);
    }
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

ElementType ActivityEdge::getElementType() {
    return ElementType::ACTIVITY_EDGE;
}

bool ActivityEdge::isSubClassOf(ElementType eType) {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_EDGE;
    }

    return ret;
}
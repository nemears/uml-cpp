#include "uml/activityEdge.h"
#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace UML;

void ActivityEdge::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_target) {
        m_target->getIncoming().reindex(oldID, newID);
    }

    if (m_source) {
        m_source->getOutgoing().reindex(oldID, newID);
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

    NamedElement::reindexName(oldName, newName);
}

Activity* ActivityEdge::getActivity() {
    return m_activity;
}

void ActivityEdge::setActivity(Activity* activity) {
    m_activity = activity;
}

ActivityNode* ActivityEdge::getSource() {
    return m_source;
}

void ActivityEdge::setSource(ActivityNode* source) {
    m_source = source;
}

ActivityNode* ActivityEdge::getTarget() {
    return m_target;
}

void ActivityEdge::setTarget(ActivityNode* target) {
    m_target = target;
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
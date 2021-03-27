#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace UML;

ActivityNode::ActivityNode() {
    m_activity = 0;
    m_incoming = new Sequence<ActivityEdge>;
    m_outgoing = new Sequence<ActivityEdge>;
}

ActivityNode::~ActivityNode() {
    delete m_incoming;
    delete m_outgoing;
}

void ActivityNode::reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) {
    if (m_activity) {
        m_activity->getNodes().reindex(oldID, newID);
    }

    NamedElement::reindexID(oldID, newID);
}

void ActivityNode::reindexName(string oldName, string newName) {
    if (m_activity) {
        m_activity->getNodes().reindex(m_id, oldName, newName);
    }

    NamedElement::reindexName(oldName, newName);
}

Sequence<ActivityEdge>& ActivityNode::getIncoming() {
    return *m_incoming;
}

Sequence<ActivityEdge>& ActivityNode::getOutgoing() {
    return *m_outgoing;
}

ElementType ActivityNode::getElementType() {
    return ElementType::ACTIVITY_NODE;
}

Activity* ActivityNode::getActivity() {
    return m_activity;
}

void ActivityNode::setActivity(Activity* activity) {
    m_activity = activity;
    if (!m_activity->getNodes().count(m_id)) {
        m_activity->getNodes().add(*this);
    }
}

bool ActivityNode::isObjectNode() {
    return false;
}
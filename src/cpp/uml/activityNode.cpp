#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace UML;

ActivityNode::ActivityNode() {
    m_incoming = new Sequence<ActivityEdge>;
    m_outgoing = new Sequence<ActivityEdge>;
}

ActivityNode::~ActivityNode() {
    delete m_incoming;
    delete m_outgoing;
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
    return activity;
}

void ActivityNode::setActivity(Activity* activity) {
    this->activity = activity;
}

bool ActivityNode::isObjectNode() {
    return false;
}
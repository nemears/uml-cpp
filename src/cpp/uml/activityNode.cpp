#include "uml/activityNode.h"
#include "uml/activity.h"

using namespace std;
using namespace UML;

ActivityNode::ActivityNode() {
    m_activity = 0;
    m_incoming = new Sequence<ActivityEdge>;
    m_incoming->addProcedures.push_back(new AddIncomingFunctor(this));
    m_incoming->addChecks.push_back(new CheckIncomingFunctor(this));
    m_outgoing = new Sequence<ActivityEdge>;
    m_outgoing->addProcedures.push_back(new AddOutgoingFunctor(this));
    m_outgoing->addChecks.push_back(new CheckOutgoingFunctor(this));
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

void ActivityNode::AddIncomingFunctor::operator()(Element& el) const {
    if (dynamic_cast<ActivityEdge&>(el).getTarget() != m_el) {
        dynamic_cast<ActivityEdge&>(el).setTarget(dynamic_cast<ActivityNode*>(m_el));
    }
}

void ActivityNode::AddOutgoingFunctor::operator()(Element& el) const {
    if (dynamic_cast<ActivityEdge&>(el).getSource() != m_el) {
        dynamic_cast<ActivityEdge&>(el).setSource(dynamic_cast<ActivityNode*>(m_el));
    }
}

void ActivityNode::CheckIncomingFunctor::operator()(Element& el) const {
    if(dynamic_cast<ActivityNode*>(m_el)->getIncoming().count(el.getID())) {
        throw DuplicateEdgeException(el.getIDstring());
    }
}

void ActivityNode::CheckOutgoingFunctor::operator()(Element& el) const {
    if(dynamic_cast<ActivityNode*>(m_el)->getOutgoing().count(el.getID())) {
        throw DuplicateEdgeException(el.getIDstring());
    }
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

bool ActivityNode::isSubClassOf(ElementType eType) {
    bool ret = NamedElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_NODE;
    }

    return ret;
}
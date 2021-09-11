#include "uml/activityNode.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"

using namespace std;
using namespace UML;

void ActivityNode::RemoveActivityProcedure::operator()(Activity* el) const {
    if (el->getNodes().count(m_me->getID())) {
        el->getNodes().remove(*m_me);
    }
}

void ActivityNode::AddActivityProcedure::operator()(Activity* el) const {
    if (!el->getNodes().count(m_me->getID())) {
        el->getNodes().add(*m_me);
    }
}

void ActivityNode::setManager(UmlManager* manager) {
    m_outgoing.m_manager = manager;
    m_incoming.m_manager = manager;
}

void ActivityNode::restoreReleased(ID id, Element* released) {
    RedefinableElement::restoreReleased(id, released);
    /** TODO: anythin? **/
}

void ActivityNode::referencingReleased(ID id) {
    RedefinableElement::referencingReleased(id);
    if (m_activity.id() == id) {
        m_activity.release();
    }
    m_incoming.elementReleased(id);
    m_outgoing.elementReleased(id);
}

ActivityNode::ActivityNode() {
    m_activity.m_signature = &ActivityNode::m_activity;
    m_activity.m_removeProcedures.push_back(new RemoveActivityProcedure(this));
    m_activity.m_addProcedures.push_back(new AddActivityProcedure(this));
    m_incoming.addProcedures.push_back(new AddIncomingFunctor(this));
    m_incoming.addChecks.push_back(new CheckIncomingFunctor(this));
    m_incoming.removeProcedures.push_back(new RemoveIncomingFunctor(this));
    m_outgoing.addProcedures.push_back(new AddOutgoingFunctor(this));
    m_outgoing.addChecks.push_back(new CheckOutgoingFunctor(this));
    m_outgoing.removeProcedures.push_back(new RemoveOutgoingFunctor(this));
}

ActivityNode::ActivityNode(const ActivityNode& rhs) : RedefinableElement(rhs), NamedElement(rhs), Element(rhs) {
    m_activity = rhs.m_activity;
    m_activity.m_me = this;
    m_activity.m_removeProcedures.clear();
    m_activity.m_addProcedures.clear();
    m_activity.m_removeProcedures.push_back(new RemoveActivityProcedure(this));
    m_activity.m_addProcedures.push_back(new AddActivityProcedure(this));
    m_incoming = rhs.m_incoming;
    m_incoming.m_el = this;
    m_incoming.addProcedures.clear();
    m_incoming.addChecks.clear();
    m_incoming.removeProcedures.clear();
    m_incoming.addProcedures.push_back(new AddIncomingFunctor(this));
    m_incoming.addChecks.push_back(new CheckIncomingFunctor(this));
    m_incoming.removeProcedures.push_back(new RemoveIncomingFunctor(this));
    m_outgoing = rhs.m_outgoing;
    m_outgoing.m_el = this;
    m_outgoing.addChecks.clear();
    m_outgoing.addProcedures.clear();
    m_outgoing.removeProcedures.clear();
    m_outgoing.addProcedures.push_back(new AddOutgoingFunctor(this));
    m_outgoing.addChecks.push_back(new CheckOutgoingFunctor(this));
    m_outgoing.removeProcedures.push_back(new RemoveOutgoingFunctor(this));
}

ActivityNode::~ActivityNode() {
    
}

void ActivityNode::reindexID(ID oldID, ID newID) {
    if (m_activity.has()) {
        m_activity.getRef().getNodes().reindex(oldID, newID);
    }
    for (auto& incoming : m_incoming) {
        incoming.m_target.reindex(oldID, newID);
    }
    for (auto& outgoing : m_outgoing) {
        outgoing.m_source.reindex(oldID, newID);
    }
    RedefinableElement::reindexID(oldID, newID);
    NamedElement::reindexID(oldID, newID);
}

// void ActivityNode::reindexName(string oldName, string newName) {
//     if (m_activity) {
//         m_activity->getNodes().reindex(m_id, oldName, newName);
//     }

//     NamedElement::reindexName(oldName, newName);
// }

void ActivityNode::AddIncomingFunctor::operator()(ActivityEdge& el) const {
    if (el.getTarget() != m_el) {
        el.setTarget(m_el);
    }
    updateCopiedSequenceAddedTo(el, &ActivityNode::getIncoming);
}

void ActivityNode::RemoveIncomingFunctor::operator()(ActivityEdge& el) const {
    if (el.getTarget() == m_el) {
        el.setTarget(0);
    }
    updateCopiedSequenceRemovedFrom(el, &ActivityNode::getIncoming);
}

void ActivityNode::AddOutgoingFunctor::operator()(ActivityEdge& el) const {
    if (el.getSource() != m_el) {
        el.setSource(m_el);
    }
    updateCopiedSequenceAddedTo(el, &ActivityNode::getOutgoing);
}

void ActivityNode::RemoveOutgoingFunctor::operator()(ActivityEdge& el) const {
    if (el.getSource() == m_el) {
        el.setSource(0);
    }
    updateCopiedSequenceRemovedFrom(el, &ActivityNode::getOutgoing);
}

void ActivityNode::CheckIncomingFunctor::operator()(ActivityEdge& el) const {
    if(m_el->getIncoming().count(el.getID())) {
        throw DuplicateEdgeException(el.getID().string());
    }
}

void ActivityNode::CheckOutgoingFunctor::operator()(ActivityEdge& el) const {
    if(m_el->getOutgoing().count(el.getID())) {
        throw DuplicateEdgeException(el.getID().string());
    }
}

Sequence<ActivityEdge>& ActivityNode::getIncoming() {
    return m_incoming;
}

Sequence<ActivityEdge>& ActivityNode::getOutgoing() {
    return m_outgoing;
}

ElementType ActivityNode::getElementType() const {
    return ElementType::ACTIVITY_NODE;
}

Activity* ActivityNode::getActivity() {
    return m_activity.get();
}

Activity& ActivityNode::getActivityRef() {
    return m_activity.getRef();
}

bool ActivityNode::hasActivity() const {
    return m_activity.has();
}

void ActivityNode::setActivity(Activity* activity) {
    m_activity.set(activity);
}

void ActivityNode::setActivity(Activity& activity) {
    m_activity.set(activity);
}

bool ActivityNode::isObjectNode() {
    return false;
}

bool ActivityNode::isSubClassOf(ElementType eType) const {
    bool ret = RedefinableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY_NODE;
    }

    return ret;
}
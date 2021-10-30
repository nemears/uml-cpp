#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/activityNode.h"

using namespace UML;

void Activity::AddNodeFunctor::operator()(ActivityNode& el) const {
    if (el.getActivity() != m_el) {
        el.setActivity(m_el);
    }

    // if owner not already specified make it the activity (don't overwrite pins ownership)
    if (!el.getOwner()) {
        if (!m_el->getOwnedElements().count(el.getID())) {
            m_el->getOwnedElements().internalAdd(el);
        }
    }

    updateCopiedSequenceAddedTo(el, &Activity::getNodes);
}

void Activity::RemoveNodeFunctor::operator()(ActivityNode& el) const {
    if (el.getActivity() == m_el) {
        el.setActivity(0);
    }

    if (m_el->getOwnedElements().count(el)) {
        m_el->getOwnedElements().internalRemove(el);
    }
    updateCopiedSequenceRemovedFrom(el, &Activity::getNodes);
}

void Activity::AddEdgeFunctor::operator()(ActivityEdge& el) const {
    if (el.getActivity() != m_el) {
        el.setActivity(m_el);
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
    updateCopiedSequenceAddedTo(el, &Activity::getEdges);
}

void Activity::RemoveEdgeFunctor::operator()(ActivityEdge& el) const {
    if (el.getActivity() == m_el) {
        el.setActivity(m_el);
    }
    subsetsRemove(el, &Element::getOwnedElements);
    updateCopiedSequenceRemovedFrom(el, &Activity::getEdges);
}

void Activity::referencingReleased(ID id) {
    Behavior::referencingReleased(id);
    m_nodes.elementReleased(id, &Activity::getNodes);
    m_edges.elementReleased(id, &Activity::getEdges);
}

void Activity::referenceReindexed(ID oldID, ID newID) {
    Behavior::referenceReindexed(oldID, newID);
    if (m_nodes.count(oldID)) {
        m_nodes.reindex(oldID, newID, &Activity::getNodes);
    }
    if (m_edges.count(oldID)) {
        m_edges.reindex(oldID, newID, &Activity::getEdges);
    }
}

Activity::Activity() : Element(ElementType::ACTIVITY) {
    m_nodes.addProcedures.push_back(new AddNodeFunctor(this));
    m_nodes.removeProcedures.push_back(new RemoveNodeFunctor(this));
    m_edges.addProcedures.push_back(new AddEdgeFunctor(this));
    m_edges.removeProcedures.push_back(new RemoveEdgeFunctor(this));
}

Activity::Activity(const Activity& rhs) : 
Behavior(rhs), 
Classifier(rhs), 
PackageableElement(rhs), 
NamedElement(rhs), 
Element(rhs, ElementType::ACTIVITY) {
    m_nodes = rhs.m_nodes;
    m_nodes.m_el = this;
    m_nodes.removeProcedures.clear();
    m_nodes.addProcedures.clear();
    m_edges = rhs.m_edges;
    m_edges.m_el = this;
    m_edges.addProcedures.clear();
    m_edges.removeProcedures.clear();
    m_nodes.addProcedures.push_back(new AddNodeFunctor(this));
    m_nodes.removeProcedures.push_back(new RemoveNodeFunctor(this));
    m_edges.addProcedures.push_back(new AddEdgeFunctor(this));
    m_edges.removeProcedures.push_back(new RemoveEdgeFunctor(this));
}

Activity::~Activity() {
    
}

Sequence<ActivityNode>& Activity::getNodes() {
    return m_nodes;
}

Sequence<ActivityEdge>& Activity::getEdges() {
    return m_edges;
}

bool Activity::isSubClassOf(ElementType eType) const {
    bool ret = Behavior::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY;
    }

    return ret;
}
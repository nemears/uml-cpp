#include "uml/activity.h"

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
}

void Activity::AddEdgeFunctor::operator()(ActivityEdge& el) const {
    if (el.getActivity() != m_el) {
        el.setActivity(m_el);
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().internalAdd(el);
    }
}

Activity::Activity() {
    m_nodes.addProcedures.push_back(new AddNodeFunctor(this));
    m_edges.addProcedures.push_back(new AddEdgeFunctor(this));
}

Activity::~Activity() {
    
}

Sequence<ActivityNode>& Activity::getNodes() {
    return m_nodes;
}

Sequence<ActivityEdge>& Activity::getEdges() {
    return m_edges;
}

ElementType Activity::getElementType() const {
    return ElementType::ACTIVITY;
}

bool Activity::isSubClassOf(ElementType eType) const {
    bool ret = Behavior::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ACTIVITY;
    }

    return ret;
}
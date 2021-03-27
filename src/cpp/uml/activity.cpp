#include "uml/activity.h"

using namespace UML;

void Activity::AddNodeFunctor::operator()(Element& el) const {
    if (dynamic_cast<ActivityNode&>(el).getActivity() != m_el) {
        dynamic_cast<ActivityNode&>(el).setActivity(dynamic_cast<Activity*>(m_el));
    }

    // if owner not already specified make it the activity (don't overwrite pins ownership)
    if (!el.getOwner()) {
        if (!m_el->getOwnedElements().count(el.getID())) {
            m_el->getOwnedElements().add(dynamic_cast<ActivityNode&>(el));
        }
    }
}

void Activity::AddEdgeFunctor::operator()(Element& el) const {
    if (dynamic_cast<ActivityEdge&>(el).getActivity() != m_el) {
        dynamic_cast<ActivityEdge&>(el).setActivity(dynamic_cast<Activity*>(m_el));
    }

    if (!m_el->getOwnedElements().count(el.getID())) {
        m_el->getOwnedElements().add(dynamic_cast<ActivityEdge&>(el));
    }
}

Activity::Activity() {
    m_nodes = new Sequence<ActivityNode>;
    m_nodes->addProcedures.push_back(new AddNodeFunctor(this));
    m_edges = new Sequence<ActivityEdge>;
    m_edges->addProcedures.push_back(new AddEdgeFunctor(this));
}

Activity::~Activity() {
    delete m_nodes;
    delete m_edges;
}

Sequence<ActivityNode>& Activity::getNodes() {
    return *m_nodes;
}

Sequence<ActivityEdge>& Activity::getEdges() {
    return *m_edges;
}

ElementType Activity::getElementType() {
    return ElementType::ACTIVITY;
}
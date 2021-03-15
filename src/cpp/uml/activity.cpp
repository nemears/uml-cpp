#include "uml/activity.h"

using namespace UML;

Activity::Activity() {
    m_nodes = new Sequence<ActivityNode>;
    m_edges = new Sequence<ActivityEdge>;
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
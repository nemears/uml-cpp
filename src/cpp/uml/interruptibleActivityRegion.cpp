#include "uml/interruptibleActivityRegion.h"
#include "uml/activity.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

void InterruptibleActivityRegion::referenceReindexed(ID newID) {
    ActivityGroup::referenceReindexed(newID);
    m_interruptingEdges.reindex(newID);
}

void InterruptibleActivityRegion::referenceErased(ID id) {
    ActivityGroup::referenceErased(id);
    m_interruptingEdges.eraseElement(id);
}

InterruptibleActivityRegion::InterruptibleActivityRegion() : Element(ElementType::INTERRUPTIBLE_ACTIVITY_REGION) {
    m_nodes.subsets(m_containedNodes);
    m_nodes.opposite(&ActivityNode::getInterruptibleRegions);
    m_interruptingEdges.opposite(&ActivityEdge::getInterruptsSingleton);
}

InterruptibleActivityRegion::~InterruptibleActivityRegion() {

}

Set<ActivityNode, InterruptibleActivityRegion>& InterruptibleActivityRegion::getNodes() {
    return m_nodes;
}

Set<ActivityEdge, InterruptibleActivityRegion>& InterruptibleActivityRegion::getInterruptingEdges() {
    return m_interruptingEdges;
}

bool InterruptibleActivityRegion::isSubClassOf(ElementType eType) const {
    bool ret = ActivityGroup::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::INTERRUPTIBLE_ACTIVITY_REGION;
    }

    return ret;
}
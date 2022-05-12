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

void InterruptibleActivityRegion::referencingReleased(ID id) {
    ActivityGroup::referencingReleased(id);
    m_interruptingEdges.release(id);
}

void InterruptibleActivityRegion::referenceReindexed(ID oldID, ID newID) {
    ActivityGroup::referenceReindexed(oldID, newID);
    m_interruptingEdges.reindex(oldID, newID);
}

void InterruptibleActivityRegion::referenceErased(ID id) {
    ActivityGroup::referenceErased(id);
    m_interruptingEdges.eraseElement(id);
}

void InterruptibleActivityRegion::reindexName(ID id, std::string newName) {
    ActivityGroup::reindexName(id, newName);
    m_interruptingEdges.reindexName(id, newName);
}

void InterruptibleActivityRegion::init() {
    m_nodes.subsets(m_containedNodes);
    m_nodes.opposite(&ActivityNode::getInterruptibleRegions);
    m_interruptingEdges.opposite(&ActivityEdge::getInterruptsSingleton);
}

InterruptibleActivityRegion::InterruptibleActivityRegion() : Element(ElementType::INTERRUPTIBLE_ACTIVITY_REGION) {
    init();
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
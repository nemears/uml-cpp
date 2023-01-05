#include "uml/executableNode.h"
#include "uml/exceptionHandler.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
#include "uml/objectNode.h"
#include "uml/umlPtr.h"

using namespace UML;

ExecutableNode::ExecutableNode() : Element(ElementType::EXECUTABLE_NODE) {
    m_handlers.subsets(*m_ownedElements);
    m_handlers.opposite(&ExceptionHandler::getProtectedNodeSingleton);
}

ExecutableNode::~ExecutableNode() {

}

Set<ExceptionHandler, ExecutableNode>& ExecutableNode::getHandlers() {
    return m_handlers;
}

bool ExecutableNode::isSubClassOf(ElementType eType) const {
    bool ret = ActivityNode::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::EXECUTABLE_NODE;
    }

    return ret;
}
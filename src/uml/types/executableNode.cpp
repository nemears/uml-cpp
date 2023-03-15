#include "uml/types/executableNode.h"
#include "uml/types/exceptionHandler.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/types/objectNode.h"
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
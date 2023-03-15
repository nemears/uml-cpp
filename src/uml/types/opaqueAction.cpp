#include "uml/types/opaqueAction.h"
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

OpaqueAction::OpaqueAction() : Element(ElementType::OPAQUE_ACTION) {
    m_inputValues.subsets(m_inputs);
    m_outputValues.subsets(m_outputs);
    m_bodies.subsets(*m_ownedElements);
}

OpaqueAction::~OpaqueAction() {

}

Set<InputPin, OpaqueAction>& OpaqueAction::getInputValues() {
    return m_inputValues;
}

Set<OutputPin, OpaqueAction>& OpaqueAction::getOutputValues() {
    return m_outputValues;
}

OrderedSet<LiteralString, OpaqueAction>& OpaqueAction::getBodies() {
    return m_bodies;
}

bool OpaqueAction::isSubClassOf(ElementType eType) const {
    bool ret = Action::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::OPAQUE_ACTION;
    }

    return ret;
}
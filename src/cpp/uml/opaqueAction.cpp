#include "uml/opaqueAction.h"
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
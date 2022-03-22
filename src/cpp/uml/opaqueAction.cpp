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

Set<LiteralString, OpaqueAction>& OpaqueAction::getBodiesSet() {
    return m_bodies;
}

void OpaqueAction::init() {
    m_inputValues.subsets(m_inputs);
    m_inputValues.m_signature = &OpaqueAction::getInputValues;
    m_outputValues.subsets(m_outputs);
    m_outputValues.m_signature = &OpaqueAction::getOutputValues;
    m_bodies.subsets(*m_ownedElements);
    m_bodies.m_signature = &OpaqueAction::getBodiesSet;
}

OpaqueAction::OpaqueAction() : Element(ElementType::OPAQUE_ACTION) {
    init();
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
#include "uml/types/activityParameterNode.h"
#include "uml/types/activity.h"
#include "uml/types/activityEdge.h"
#include "uml/types/package.h"
#include "uml/types/property.h"
#include "uml/types/generalization.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/interruptibleActivityRegion.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Parameter, ActivityParameterNode>& ActivityParameterNode::getParameterSingleton() {
    return m_parameter;
}

void ActivityParameterNode::referenceReindexed(ID newID) {
    ObjectNode::referenceReindexed(newID);
    m_parameter.reindex(newID);
}

void ActivityParameterNode::referenceErased(ID id) {
    ObjectNode::referenceErased(id);
    m_parameter.eraseElement(id);
}

ActivityParameterNode::ActivityParameterNode() : Element(ElementType::ACTIVITY_PARAMETER_NODE) {
    
}

ActivityParameterNode::~ActivityParameterNode() {
    
}

ParameterPtr ActivityParameterNode::getParameter() const {
    return m_parameter.get();
}

void ActivityParameterNode::setParameter(Parameter* parameter) {
    m_parameter.set(parameter);
}

void ActivityParameterNode::setParameter(Parameter& parameter) {
    m_parameter.set(parameter);
}

void ActivityParameterNode::setParameter(ParameterPtr parameter) {
    m_parameter.set(parameter);
}

void ActivityParameterNode::setParameter(ID id) {
    m_parameter.set(id);
}

bool ActivityParameterNode::isSubClassOf(ElementType eType) const {
    bool ret = ObjectNode::isSubClassOf(eType);

    if(!ret) {
        ret = eType == ElementType::ACTIVITY_PARAMETER_NODE;
    }

    return ret;
}
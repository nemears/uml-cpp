#include "uml/activityParameterNode.h"
#include "uml/activity.h"
#include "uml/activityEdge.h"
#include "uml/package.h"
#include "uml/property.h"
#include "uml/generalization.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/interruptibleActivityRegion.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"
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
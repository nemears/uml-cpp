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
#include "uml/setReferenceFunctor.h"

using namespace UML;

Set<Parameter, ActivityParameterNode>& ActivityParameterNode::getParameterSingleton() {
    return m_parameter;
}

void ActivityParameterNode::referencingReleased(ID id) {
    ObjectNode::referencingReleased(id);
    m_parameter.release(id);
}

void ActivityParameterNode::referenceReindexed(ID oldID, ID newID) {
    ObjectNode::referenceReindexed(oldID, newID);
    m_parameter.reindex(oldID, newID);
}

void ActivityParameterNode::reindexName(ID id, std::string newName) {
    ObjectNode::reindexName(id, newName);
    m_parameter.reindexName(id, newName);
}

void ActivityParameterNode::referenceErased(ID id) {
    ObjectNode::referenceErased(id);
    m_parameter.eraseElement(id);
}

void ActivityParameterNode::init() {
    m_parameter.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_parameter.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
}

ActivityParameterNode::ActivityParameterNode() : Element(ElementType::ACTIVITY_PARAMETER_NODE) {
    init();
}

ActivityParameterNode::~ActivityParameterNode() {}

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
#include "uml/types/feature.h"
#include "uml/types/namedElement.h"
#include "uml/types/namespace.h"
#include "uml/types/redefinableElement.h"
#include "uml/uml-stable.h"

using namespace UML;

BehavioralFeature::BehavioralFeature(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    RedefinableElement(elementType, manager),
    Feature(elementType, manager),
    Namespace(elementType, manager)
{
    m_methods.opposite(&Behavior::getSpecificationSingleton);
    m_ownedParameters.subsets(m_ownedMembers);
    m_ownedParameterSets.subsets(m_ownedElements);
}

Set<Behavior, BehavioralFeature>& BehavioralFeature::getMethods() {
    return m_methods;
}

Set<Parameter, BehavioralFeature>& BehavioralFeature::getOwnedParameters() {
    return m_ownedParameters;
}

Set<Type, BehavioralFeature>& BehavioralFeature::getRaisedExceptions() {
    return m_raisedExceptions;
}

Set<ParameterSet, BehavioralFeature>& BehavioralFeature::getOwnedParameterSets() {
    return m_ownedParameterSets;
}

bool BehavioralFeature::isAbstract() {
    return m_methods.size() > 0;
}

CallConcurrencyKind BehavioralFeature::getConcurrency() const {
    return m_concurrency;
}

void BehavioralFeature::setConcurrency(CallConcurrencyKind concurrency) {
    m_concurrency = concurrency;
}

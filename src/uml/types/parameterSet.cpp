#include "uml/uml-stable.h"

using namespace UML;

ParameterSet::ParameterSet(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager) 
{
    m_conditions.subsets(m_ownedElements);
    m_parameters.opposite(&Parameter::getParameterSets);
}

Set<Constraint, ParameterSet>& ParameterSet::getConditions() {
    return m_conditions;
}

Set<Parameter, ParameterSet>& ParameterSet::getParameters() {
    return m_parameters;
}

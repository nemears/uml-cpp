#include "uml/uml-stable.h"

using namespace UML;

void ParameterSet::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    eraseFromSet(id, m_parameters);
}

ParameterSet::ParameterSet() : Element(ElementType::PARAMETER_SET) {
    m_conditions.subsets(*m_ownedElements);
    m_parameters.opposite(&Parameter::getParameterSets);
}

ParameterSet::~ParameterSet() {

}

Set<Constraint, ParameterSet>& ParameterSet::getConditions() {
    return m_conditions;
}

Set<Parameter, ParameterSet>& ParameterSet::getParameters() {
    return m_parameters;
}

bool ParameterSet::is(ElementType eType) const {
    if (NamedElement::is(eType)) {
        return true;
    }

    return eType == ElementType::PARAMETER_SET;
}

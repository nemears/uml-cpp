#include "uml/types/parameterSet.h"
#include "uml/types/parameter.h"
#include "uml/types/package.h"
#include "uml/types/dataType.h"
#include "uml/types/behavior.h"
#include "uml/types/operation.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/interface.h"
#include "uml/types/deployment.h"

using namespace UML;

void ParameterSet::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_parameters.eraseElement(id);
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

bool ParameterSet::isSubClassOf(ElementType eType) const {
    if (NamedElement::isSubClassOf(eType)) {
        return true;
    }

    return eType == ElementType::PARAMETER_SET;
}
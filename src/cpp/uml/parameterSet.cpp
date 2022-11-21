#include "uml/parameterSet.h"
#include "uml/parameter.h"
#include "uml/package.h"
#include "uml/dataType.h"
#include "uml/behavior.h"
#include "uml/operation.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/interface.h"
#include "uml/deployment.h"

using namespace UML;

void ParameterSet::referenceReindexed(ID oldID, ID newID) {
    NamedElement::referenceReindexed(oldID, newID);
    m_parameters.reindex(newID);
}

void ParameterSet::referenceErased(ID id) {
    NamedElement::referenceErased(id);
    m_parameters.eraseElement(id);
}

void ParameterSet::init() {
    m_conditions.subsets(*m_ownedElements);
    m_parameters.opposite(&Parameter::getParameterSets);
}

ParameterSet::ParameterSet() : Element(ElementType::PARAMETER_SET) {
    init();
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
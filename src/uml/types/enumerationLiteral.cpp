#include "uml/types/enumerationLiteral.h"
#include "uml/types/enumeration.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Enumeration, EnumerationLiteral>& EnumerationLiteral::getEnumerationSingleton() {
    return m_enumeration;
}

EnumerationLiteral::EnumerationLiteral() : Element(ElementType::ENUMERATION_LITERAL) {
    m_enumeration.subsets(*m_namespace);
    m_enumeration.opposite(&Enumeration::getOwnedLiterals);
}

EnumerationLiteral::~EnumerationLiteral() {
    
}

EnumerationPtr EnumerationLiteral::getEnumeration() const {
    return m_enumeration.get();
}

void EnumerationLiteral::setEnumeration(Enumeration* enumeration) {
    m_enumeration.set(enumeration);
}

void EnumerationLiteral::setEnumeration(Enumeration& enumeration) {
    m_enumeration.set(enumeration);
}

void EnumerationLiteral::setEnumeration(ID id) {
    m_enumeration.set(id);
}

bool EnumerationLiteral::isSubClassOf(ElementType eType) const {
    bool ret = InstanceSpecification::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION_LITERAL;
    }

    return ret;
}
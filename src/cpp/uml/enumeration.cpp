#include "uml/enumeration.h"
#include "uml/enumerationLiteral.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

Enumeration::Enumeration() : Element(ElementType::ENUMERATION) {
    m_ownedLiterals.subsets(m_ownedMembers);
    m_ownedLiterals.opposite(&EnumerationLiteral::getEnumerationSingleton);
}

Enumeration::~Enumeration() {
    
}

OrderedSet<EnumerationLiteral, Enumeration>& Enumeration::getOwnedLiterals() {
    return m_ownedLiterals;
}

bool Enumeration::isSubClassOf(ElementType eType) const {
    bool ret = DataType::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION;
    }

    return ret;
}
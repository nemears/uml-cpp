#include "uml/types/enumeration.h"
#include "uml/types/enumerationLiteral.h"
#include "uml/types/behavior.h"
#include "uml/types/association.h"
#include "uml/types/stereotype.h"
#include "uml/types/deployment.h"

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
#include "uml/uml-stable.h"

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

bool Enumeration::is(ElementType eType) const {
    bool ret = DataType::is(eType);

    if (!ret) {
        ret = eType == ElementType::ENUMERATION;
    }

    return ret;
}

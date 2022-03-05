#include "uml/enumeration.h"
#include "uml/enumerationLiteral.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"

using namespace UML;

Set<EnumerationLiteral, Enumeration>& Enumeration::getOwnedLiteralsSet() {
    return m_ownedLiterals;
}

void Enumeration::init() {
    m_ownedLiterals.subsets(m_ownedMembers);
    m_ownedLiterals.opposite(&EnumerationLiteral::getEnumerationSingleton);
    m_ownedLiterals.m_signature = &Enumeration::getOwnedLiteralsSet;
}

Enumeration::Enumeration() : Element(ElementType::ENUMERATION) {
    init();
}

Enumeration::~Enumeration() {
    mountAndRelease();
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
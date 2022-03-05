#include "uml/enumerationLiteral.h"
#include "uml/enumeration.h"
#include "uml/behavior.h"
#include "uml/association.h"
#include "uml/stereotype.h"
#include "uml/deployment.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<Enumeration, EnumerationLiteral>& EnumerationLiteral::getEnumerationSingleton() {
    return m_enumeration;
}

void EnumerationLiteral::init() {
    m_enumeration.subsets(m_namespace);
    m_enumeration.opposite(&Enumeration::getOwnedLiteralsSet);
    m_enumeration.m_signature = &EnumerationLiteral::getEnumerationSingleton;
}

EnumerationLiteral::EnumerationLiteral() : Element(ElementType::ENUMERATION_LITERAL) {
    init();
}

EnumerationLiteral::~EnumerationLiteral() {
    mountAndRelease();
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
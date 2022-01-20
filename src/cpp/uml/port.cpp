#include "uml/port.h"
#include "uml/uml-stable.h"

using namespace UML;

void Port::copy(const Port& rhs) {
    m_isBehavior = rhs.m_isBehavior;
    m_isConjugated = rhs.m_isConjugated;
    m_isService = rhs.m_isService;
}

Port::Port() : Element(ElementType::PORT) {

}

Port::~Port() {

}

Port::Port(const Port& rhs) : Element(rhs, ElementType::PORT) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    TypedElement::copy(rhs);
    MultiplicityElement::copy(rhs);
    RedefinableElement::copy(rhs);
    Feature::copy(rhs);
    DeploymentTarget::copy(rhs);
    Property::copy(rhs);
    copy(rhs);
}

bool Port::isBehavior() const {
    return m_isBehavior;
}

void Port::setIsBehavior(bool isBehavior) {
    m_isBehavior = isBehavior;
    updateCopiesScalar(isBehavior, &Port::m_isBehavior);
}

bool Port::isConjugated() const {
    return m_isConjugated;
}

void Port::setIsConjugated(bool isConjugated) {
    m_isConjugated = isConjugated;
    updateCopiesScalar(isConjugated, &Port::m_isConjugated);
}

bool Port::isService() const {
    return m_isService;
}

void Port::setIsService(bool isService) {
    m_isService = isService;
    updateCopiesScalar(isService, &Port::m_isService);
}

bool Port::isSubClassOf(ElementType eType) const {
    bool ret = Property::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::PORT;
    }

    return ret;
}
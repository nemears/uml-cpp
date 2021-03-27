#include "uml/generalization.h"

using namespace UML;

Generalization::Generalization() {
    m_general = 0;
    m_specific = 0;
}

Classifier* Generalization::getGeneral() {
    return m_general;
}

void Generalization::setGeneral(Classifier* general) {
    m_general = general;
    if (!getTargets().count(m_general->getID())) {
        getTargets().add(*m_general);
    }
}

Classifier* Generalization::getSpecific() {
    return m_specific;
}

void Generalization::setSpecific(Classifier* specific) {
    m_specific = specific;
    if (!getSources().count(m_specific->getID())) {
        getSources().add(*m_specific);
    }
}

ElementType Generalization::getElementType() {
    return ElementType::GENERALIZATION;
}
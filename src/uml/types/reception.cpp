#include "uml/types/reception.h"
#include "uml/types/operation.h"
#include "uml/types/manifestation.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"
#include "uml/types/signal.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Signal, Reception>& Reception::getSignalSingleton() {
    return m_signal;
}

void Reception::referenceErased(ID id) {
    BehavioralFeature::referenceErased(id);
    m_signal.eraseElement(id);
}

void Reception::restoreReferences() {
    BehavioralFeature::restoreReferences();
    if (m_namespace->get().id() != ID::nullID() && m_featuringClassifier.get().id() == ID::nullID()) {
        m_featuringClassifier.innerAdd(m_namespace->get().id());
    }
}

Reception::Reception() : Element(ElementType::RECEPTION) {
    
}

Reception::~Reception() {
    
}

SignalPtr Reception::getSignal() const {
    return m_signal.get();
}

void Reception::setSignal(Signal* signal) {
    m_signal.set(signal);
}

void Reception::setSignal(Signal& signal) {
    m_signal.set(signal);
}

void Reception::setSignal(ID id) {
    m_signal.set(id);
}

bool Reception::isSubClassOf(ElementType eType) const {
    bool ret = BehavioralFeature::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::RECEPTION;
    }

    return ret;
}
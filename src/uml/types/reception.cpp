#include "uml/set/singleton.h"
#include "uml/uml-stable.h"

using namespace UML;

Singleton<Signal, Reception>& Reception::getSignalSingleton() {
    return m_signal;
}

void Reception::referenceErased(ID id) {
    BehavioralFeature::referenceErased(id);
    eraseFromSet(id, m_signal);
}

void Reception::restoreReferences() {
    BehavioralFeature::restoreReferences();
    if (m_namespace.get().id() != ID::nullID() && m_featuringClassifier.get().id() == ID::nullID()) {
        m_featuringClassifier.nonOppositeAdd(m_namespace.get());
    }
}

Reception::Reception() : Element(ElementType::RECEPTION) {
    
}

Reception::~Reception() {
    
}

SignalPtr Reception::getSignal() const {
    return m_signal.get();
}

void Reception::setSignal(SignalPtr signal) {
    m_signal.set(signal);
}

void Reception::setSignal(Signal& signal) {
    m_signal.set(signal);
}

void Reception::setSignal(ID id) {
    m_signal.set(id);
}

bool Reception::is(ElementType eType) const {
    bool ret = BehavioralFeature::is(eType);

    if (!ret) {
        ret = eType == ElementType::RECEPTION;
    }

    return ret;
}

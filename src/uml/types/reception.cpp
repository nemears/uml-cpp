#include "uml/uml-stable.h"

using namespace UML;

Singleton<Signal, Reception>& Reception::getSignalSingleton() {
    return m_signal;
}

Reception::Reception(std::size_t elementType, AbstractManager& manager) : 
    Element(elementType, manager),
    NamedElement(elementType, manager),
    RedefinableElement(elementType, manager),
    BehavioralFeature(elementType, manager)
{}

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

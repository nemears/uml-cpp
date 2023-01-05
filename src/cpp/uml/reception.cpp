#include "uml/reception.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/signal.h"
#include "uml/umlPtr.h"

using namespace UML;

TypedSet<Signal, Reception>& Reception::getSignalSingleton() {
    return m_signal;
}

void Reception::referenceReindexed(ID newID) {
    BehavioralFeature::referenceReindexed(newID);
    m_signal.reindex(newID);
}

void Reception::referenceErased(ID id) {
    BehavioralFeature::referenceErased(id);
    m_signal.eraseElement(id);
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
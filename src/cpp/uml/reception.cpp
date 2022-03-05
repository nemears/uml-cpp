#include "uml/reception.h"
#include "uml/operation.h"
#include "uml/manifestation.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"
#include "uml/signal.h"
#include "uml/setReferenceFunctor.h"
#include "uml/umlPtr.h"

using namespace UML;

Set<Signal, Reception>& Reception::getSignalSingleton() {
    return m_signal;
}

void Reception::referencingReleased(ID id) {
    m_signal.release(id);
}

void Reception::referenceReindexed(ID oldID, ID newID) {
    m_signal.reindex(oldID, newID);
}

void Reception::reindexName(std::string oldName, std::string newName) {
    m_signal.reindexName(oldName, newName);
}

void Reception::referenceErased(ID id) {
    m_signal.eraseElement(id);
}

void Reception::init() {
    m_signal.m_signature = &Reception::getSignalSingleton;
    m_signal.m_addFunctors.insert(new SetReferenceFunctor(this));
    m_signal.m_removeFunctors.insert(new RemoveReferenceFunctor(this));
}

Reception::Reception() : Element(ElementType::RECEPTION) {
    init();
}

Reception::~Reception() {
    mountAndRelease();
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
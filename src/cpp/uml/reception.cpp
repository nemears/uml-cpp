#include "uml/reception.h"
#include "uml/uml-stable.h"
#include "uml/setReferenceFunctor.h"

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

void Reception::copy(const Reception& rhs) {
    m_signal = rhs.m_signal;
}

Reception::Reception() : Element(ElementType::RECEPTION) {
    init();
}

Reception::Reception(const Reception& rhs) : Element(rhs, ElementType::RECEPTION) {
    init();
    Element::copy(rhs);
    NamedElement::copy(rhs);
    RedefinableElement::copy(rhs);
    Feature::copy(rhs);
    BehavioralFeature::copy(rhs);
    copy(rhs);
}

Reception::~Reception() {

}

Signal* Reception::getSignal() {
    return m_signal.get();
}

Signal& Reception::getSignalRef() {
    return m_signal.getRef();
}

bool Reception::hasSignal() const {
    return m_signal.has();
}

ID Reception::getSignalID() const {
    return m_signal.id();
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
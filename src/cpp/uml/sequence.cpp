#include "uml/sequence.h"

using namespace UML;

void Sequence::add(Element& el) {
    if (!m_data.count(el.uuid)) {
        m_data[el.uuid] = &el;
        // TODO add to name_translations
    } else {
        // TODO throw error
    }
}

void Sequence::remove(Element& el) {
    if (m_data.count(el.uuid)) {
        m_data.erase(el.uuid);
        // TODO remove from name_translations
    } else {
        // TODO error or warning
    }
}

Element* Sequence::get(boost::uuids::uuid id) {
    return m_data[id];
}

Element* Sequence::get(string name) {
    return m_nameTranslation[name];
}

SequenceIterator Sequence::iterator() {
    return SequenceIterator(this);
}

Element* SequenceIterator::getNext() {
    if (m_curr != m_end) {
        ++m_curr;
        return (*m_curr).second;
    } else {
        return nullptr;
    }
}

bool SequenceIterator::hasNext() {
    return m_curr != m_end;
}
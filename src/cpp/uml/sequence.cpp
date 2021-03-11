#include "uml/sequence.h"

using namespace UML;

void Sequence::add(Element& el) {
    if (!m_data.count(el.uuid)) {
        m_data[el.uuid] = &el;
        m_order.push_back(el.uuid);
        // TODO add to name_translations
    } else {
        // TODO throw error
    }
}

void Sequence::remove(Element& el) {
    if (m_data.count(el.uuid)) {
        m_data.erase(el.uuid);
        vector<boost::uuids::uuid>::iterator orderIt = m_order.begin();
        while((*orderIt) != el.uuid && orderIt != m_order.end()) {
            ++orderIt;
        }
        if ((*orderIt) == el.uuid) {
            m_order.erase(orderIt);
        } else {
            // TODO error
        }
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

size_t Sequence::size() {
    return m_data.size();
}

SequenceIterator Sequence::iterator() {
    return SequenceIterator(this);
}

Element* SequenceIterator::getNext() {
    Element* ret = m_sequence->m_data[(*m_curr)];
    ++m_curr;
    return ret;
}

bool SequenceIterator::hasNext() {
    return m_curr != m_end;
}
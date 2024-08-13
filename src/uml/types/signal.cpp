#include "uml/uml-stable.h"

using namespace UML;

Signal::Signal() : Element(ElementType::SIGNAL) {
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
}

Signal::~Signal() {
    
}

IndexableOrderedSet<Property, Signal>& Signal::getOwnedAttributes() {
    return m_ownedAttributes;
}

bool Signal::is(ElementType eType) const {
    bool ret = Classifier::is(eType);
    
    if (!ret) {
        ret = eType == ElementType::SIGNAL;
    }

    return ret;
}

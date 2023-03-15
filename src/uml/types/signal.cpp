#include "uml/types/signal.h"
#include "uml/types/interface.h"
#include "uml/types/operation.h"
#include "uml/types/manifestation.h"
#include "uml/types/stereotype.h"
#include "uml/types/behavior.h"
#include "uml/types/dataType.h"
#include "uml/types/association.h"
#include "uml/types/deployment.h"

using namespace UML;

Signal::Signal() : Element(ElementType::SIGNAL) {
    m_ownedAttributes.subsets(m_attributes);
    m_ownedAttributes.subsets(m_ownedMembers);
}

Signal::~Signal() {
    
}

OrderedSet<Property, Signal>& Signal::getOwnedAttributes() {
    return m_ownedAttributes;
}

bool Signal::isSubClassOf(ElementType eType) const {
    bool ret = Classifier::isSubClassOf(eType);
    
    if (!ret) {
        ret = eType == ElementType::SIGNAL;
    }

    return ret;
}
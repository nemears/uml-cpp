#include "uml/managers/abstractElement.h"
#include "uml/managers/abstractManager.h"
#include "uml/set/abstractSet.h"

using namespace UML;

void AbstractElement::setID(ID id) {
    m_manager.reindex(m_id, id);
    m_id = id;
}

void AbstractElement::addToReadonlySet(AbstractSet& set, AbstractElement& el) {
    set.innerAdd(&el);
}

void AbstractElement::removeFromReadonlySet(AbstractSet& set, AbstractElement& el) {
    set.innerRemove(&el);
}

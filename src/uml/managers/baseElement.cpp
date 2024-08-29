#include "uml/managers/baseElement.h"
#include "uml/managers/abstractManager.h"

using namespace UML;

void AbstractElement::setID(ID id) {
    m_manager.reindex(m_id, id);
    m_id = id;
}

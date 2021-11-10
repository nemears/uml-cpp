#include "uml/set.h"

using namespace UML;

void AbstractContainer::setName(ContainerNode* node) {
    if (node->m_el) {
        if (node->m_el->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            node->m_name = dynamic_cast<Namedelement*>(node->m_el)->getName();
        }
    }
}
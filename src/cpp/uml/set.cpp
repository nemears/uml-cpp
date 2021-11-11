#include "uml/set.h"
#include "uml/namedElement.h"

using namespace UML;

void AbstractSet::setName(ContainerNode* node) {
    if (node->m_el) {
        if (node->m_el->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            node->m_name = dynamic_cast<NamedElement*>(node->m_el)->getName();
        }
    }
}
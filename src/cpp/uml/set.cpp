#include "uml/set.h"
#include "uml/namedElement.h"
#include "uml/namespace.h"

using namespace UML;

void AbstractSet::setName(SetNode* node) {
    if (node->m_el) {
        if (node->m_el->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            node->m_name = dynamic_cast<NamedElement*>(node->m_el)->getName();
        }
    }
}
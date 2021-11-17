#include "uml/set.h"
#include "uml/namedElement.h"

using namespace UML;

AbstractSet::SetNode::SetNode(Element* el) : m_el(el) {
    m_id = el->getID();
};

AbstractSet::SetNode::SetNode(void* el) : m_el(reinterpret_cast<Element*>(el)) {
    m_id = m_el->getID();
};

void AbstractSet::setName(SetNode* node) {
    if (node->m_el) {
        if (node->m_el->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            node->m_name = dynamic_cast<NamedElement*>(node->m_el)->getName();
        }
    }
}
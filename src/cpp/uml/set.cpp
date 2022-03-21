#include "uml/set.h"
#include "uml/namedElement.h"
#include "uml/namespace.h"
#include "uml/stereotype.h"
#include "uml/behavior.h"
#include "uml/dataType.h"
#include "uml/association.h"
#include "uml/deployment.h"

using namespace UML;

void AbstractSet::setName(SetNode* node) {
    if (node->m_el) {
        if (node->m_el->isSubClassOf(ElementType::NAMED_ELEMENT)) {
            node->m_name = dynamic_cast<NamedElement*>(node->m_el)->getName();
        }
    }
}
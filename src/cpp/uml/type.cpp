#include "uml/type.h"
#include "uml/typedElement.h"

using namespace UML;

void Type::reindexID(ID oldID, ID newID) {
    if (m_node) {
        for (auto& ref : m_node->m_references) {
            // TODO change to polymorphic?
            if (ref.second->m_managerElementMemory->isSubClassOf(ElementType::TYPED_ELEMENT)) {
                if (ref.second->m_managerElementMemory->as<TypedElement>().hasType()) {
                    if (ref.second->m_managerElementMemory->as<TypedElement>().getTypeRef() == *this) {
                        ref.second->m_managerElementMemory->as<TypedElement>().m_type.m_id = newID;
                        ref.second->m_managerElementMemory->as<TypedElement>().m_type.m_ptr = 0;
                        ref.second->m_managerElementMemory->as<TypedElement>().m_type.updateCopies();
                    }
                }
            }
        }
    }
}

ElementType Type::getElementType() const {
    return ElementType::TYPE;
}

bool Type::isSubClassOf(ElementType eType) const {
    bool ret = PackageableElement::isSubClassOf(eType);

    if (!ret) {
        ret = eType == ElementType::TYPE;
    }

    return ret;
}
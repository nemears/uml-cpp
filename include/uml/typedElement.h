#ifndef TYPEDELEMENTH
#define TYPEDELEMENTH
#include "namedElement.h"
#include "type.h"

namespace UML{
class TypedElement : virtual public NamedElement {
    protected:
        Type* m_typePtr;
        ID m_typeID;
    public:
        virtual Type* getType();
        virtual void setType(Type* type);
        TypedElement();
        TypedElement(const TypedElement& el);
        ElementType getElementType() const override;
        TypedElement& operator=(TypedElement&&) {
            return *this;
        };
        bool isSubClassOf(ElementType eType) const override;
        static ElementType elementType() {
            return ElementType::TYPED_ELEMENT;
        };
};
}

#endif
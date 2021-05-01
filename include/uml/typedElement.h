#ifndef TYPEDELEMENTH
#define TYPEDELEMENTH
#include "namedElement.h"
#include "type.h"

namespace UML{
class TypedElement : virtual public NamedElement {
    protected:
        Type* m_type;

    public:
        Type* getType();
        virtual void setType(Type* type);
        TypedElement() {
            m_type = 0;
        };
        TypedElement(const TypedElement& el);
        ElementType getElementType() const override;
        TypedElement& operator=(TypedElement&&) {
            return *this;
        };
        bool isSubClassOf(ElementType eType) override;
};
}

#endif
#ifndef TYPED_ELEMENT_H
#define TYPED_ELEMENT_H

#include "namedElement.h"

namespace UML{

    class Type;

    class TypedElement : virtual public NamedElement {
        protected:
            Singleton<Type, TypedElement> m_type = Singleton<Type, TypedElement>(this);
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            virtual Type* getType();
            Type& getTypeRef();
            bool hasType();
            virtual void setType(Type* type);
            void setType(Type& type);
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
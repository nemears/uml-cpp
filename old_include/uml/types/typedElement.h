#pragma once

#include "namedElement.h"
#include "uml/set/singleton.h"

namespace UML{

    class Type;
    typedef UmlPtr<Type> TypePtr;

    class TypedElement : virtual public NamedElement {

        friend class Type;

        protected:
            CustomSingleton<Type, TypedElement> m_type = CustomSingleton<Type, TypedElement>(this);
            void referenceErased(ID id) override;
            TypedSet<Type, TypedElement>& getTypeSingleton();
            TypedElement();
        public:
            virtual ~TypedElement();
            virtual TypePtr getType() const;
            virtual void setType(Type* type);
            void setType(Type& type);
            void setType(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TYPED_ELEMENT;
            };
    };
}
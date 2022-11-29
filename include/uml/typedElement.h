#ifndef _UML_TYPED_ELEMENT_H_
#define _UML_TYPED_ELEMENT_H_

#include "namedElement.h"
#include "set/singleton.h"

namespace UML{

    class Type;
    typedef UmlPtr<Type> TypePtr;

    class TypedElement : virtual public NamedElement {

        friend class Type;

        protected:
            CustomSingleton<Type, TypedElement> m_type = CustomSingleton<Type, TypedElement>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            void restoreReference(Element* el) override;
            TypedSet<Type, TypedElement>& getTypeSingleton();
            void init();
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

#endif
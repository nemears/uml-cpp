#ifndef TYPED_ELEMENT_H
#define TYPED_ELEMENT_H

#include "namedElement.h"

namespace UML{

    class Type;

    class TypedElement : virtual public NamedElement {

        friend class Type;

        protected:
            Singleton<Type, TypedElement> m_type = Singleton<Type, TypedElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReference(Element* el) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<Type, TypedElement>& getTypeSingleton();
            void init();
            void copy(const TypedElement& rhs);
            TypedElement();
        public:
            TypedElement(const TypedElement& el);
            virtual ~TypedElement();
            virtual Type* getType();
            Type& getTypeRef();
            ID getTypeID() const;
            bool hasType() const;
            virtual void setType(Type* type);
            void setType(Type& type);
            void setType(ID id);
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
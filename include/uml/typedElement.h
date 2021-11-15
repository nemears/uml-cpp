#ifndef TYPED_ELEMENT_H
#define TYPED_ELEMENT_H

#include "namedElement.h"

namespace UML{

    class Type;
    namespace Parsers {
        class SetType;
    }

    class TypedElement : virtual public NamedElement {

        friend class Type;
        friend class Parsers::SetType;

        protected:
            Singleton2<Type, TypedElement> m_type = Singleton2<Type, TypedElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
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
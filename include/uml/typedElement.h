#ifndef TYPED_ELEMENT_H
#define TYPED_ELEMENT_H

#include "namedElement.h"

namespace UML{

    class Type;

    class TypedElement : virtual public NamedElement {

        friend class Type;

        protected:
            Singleton<Type, TypedElement> m_type = Singleton<Type, TypedElement>(this);
            class RemoveTypeProcedure : public AbstractSingletonProcedure<Type, TypedElement> {
                public:
                    RemoveTypeProcedure(TypedElement* me) : AbstractSingletonProcedure<Type, TypedElement>(me) {};
                    void operator()(Type* el) const override;
            };
            class AddTypeProcedure : public AbstractSingletonProcedure<Type, TypedElement> {
                public:
                    AddTypeProcedure(TypedElement* me) : AbstractSingletonProcedure<Type, TypedElement>(me) {};
                    void operator()(Type* el) const override;
            };
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            TypedElement();
        public:
            TypedElement(const TypedElement& el);
            virtual ~TypedElement();
            virtual Type* getType();
            Type& getTypeRef();
            bool hasType() const;
            virtual void setType(Type* type);
            void setType(Type& type);
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
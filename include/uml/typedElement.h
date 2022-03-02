#ifndef _UML_TYPED_ELEMENT_H_
#define _UML_TYPED_ELEMENT_H_

#include "namedElement.h"

namespace UML{

    class Type;
    typedef UmlPtr<Type> TypePtr;

    class TypedElement : virtual public NamedElement {

        friend class Type;

        protected:
            Singleton<Type, TypedElement> m_type = Singleton<Type, TypedElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            Set<Type, TypedElement>& getTypeSingleton();
            void init();
            TypedElement();
        public:
            virtual ~TypedElement();
            virtual TypePtr getType();
            virtual void setType(Type* type);
            void setType(Type& type);
            void setType(ID id);
            // TypedElement& operator=(TypedElement&&) {
            //     return *this;
            // };
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TYPED_ELEMENT;
            };
    };
}

#endif
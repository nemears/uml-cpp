#pragma once

#include "namedElement.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML{

    class Type;
    typedef UmlPtr<Type> TypePtr;

    class TypedElement : virtual public NamedElement {

        friend class Type;
        friend struct ElementInfo<TypedElement>;

        protected:
            Singleton<Type, TypedElement> m_type = Singleton<Type, TypedElement>(this);
            Singleton<Type, TypedElement>& getTypeSingleton();
            TypedElement(std::size_t elementType, AbstractManager& manager);
        public:
            TypePtr getType() const;
            void setType(TypePtr type);
            void setType(Type& type);
            void setType(ID id);
            typedef TypeInfo<std::tuple<NamedElement>, TypedElement> Info;
    };

    template <>
    struct ElementInfo<TypedElement> : public DefaultInfo {
        static const bool abstract = true;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "TypedElement"; }
        static SetList sets(TypedElement& el) {
            return SetList {
                makeSetPair("type", el.m_type)
            };
        }
    };
}

#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class NamedElement;

    template <class>
    class Type;

    template <class ManagerPolicy>
    class TypedElement : public ManagerPolicy {
        friend struct EGM::ElementInfo<TypedElement>;
        public:
            using Info = EGM::TypeInfo<TypedElement, EGM::TemplateTypeList<NamedElement>>;
        protected:
            using TypeSingleton = EGM::Singleton<Type, TypedElement>;
            TypeSingleton m_type = TypeSingleton(this);
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(TypedElement);

            TypeSingleton& getTypeSingleton() { return m_type; }
            using TypeImpl = typename TypeSingleton::ManagedType;
            using TypePtr = EGM::ManagedPtr<TypeImpl>;
            TypePtr getType() { return m_type.get(); }
            void setType(TypeImpl& el) { m_type.set(el); }
            void setType(TypePtr ptr) { m_type.set(ptr); }
            void setType(EGM::ID id) { m_type.set(id); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::TypedElement> {
        static const bool abstract = true;
        static std::string name() { return "TypedElement"; }
        template <class Policy>
        static SetList sets(UML::TypedElement<Policy>& el) {
            return SetList {
                make_set_pair("type", el.m_type)
            };
        }
    };
}

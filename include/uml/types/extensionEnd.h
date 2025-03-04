#pragma once

#include "egm/egm-basic.h"
#include "egm/managedPtr.h"
#include "egm/manager/typeInfo.h"

namespace UML {
    template <class>
    class Property;

    template <class>
    class Stereotype;

    template <class ManagerPolicy>
    class ExtensionEnd : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<ExtensionEnd, EGM::TemplateTypeList<Property>>;
        protected:
            using TypeSingleton = EGM::Singleton<Stereotype, ExtensionEnd>;
            TypeSingleton m_extensionEndType = TypeSingleton(this);
        private:
            void init() {
                m_extensionEndType.redefines(this->m_type);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(ExtensionEnd);

            TypeSingleton& getTypeSingleton() { return m_extensionEndType; }
            using StereotypeImpl = typename TypeSingleton::ManagedType;
            using StereotypePtr = EGM::ManagedPtr<StereotypeImpl>;
            StereotypePtr getType() const { return m_extensionEndType.get(); }
            void setType(StereotypeImpl& el) { m_extensionEndType.set(el); }
            void setType(StereotypePtr el) { m_extensionEndType.set(el); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::ExtensionEnd> {
        static std::string name() { return "ExtensionEnd"; }
        template <class Policy>
        static SetList sets(UML::ExtensionEnd<Policy>& el) {
            return SetList {
                make_set_pair("type", el.getTypeSingleton())
            };
        }
    };
}

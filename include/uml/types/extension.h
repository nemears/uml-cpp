#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Association;

    template <class>
    class ExtensionEnd;

    template <class ManagerPolicy>
    class Extension : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Extension, EGM::TemplateTypeList<Association>>;
        protected:
            using OwnedEnd = EGM::Singleton<ExtensionEnd, Extension>;
            OwnedEnd m_extensionOwnedEnds = OwnedEnd(this);
        private:
            void init() {
                m_extensionOwnedEnds.redefines(this->m_ownedEnds);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Extension);

            OwnedEnd& getOwnedEndSingleton() { return m_extensionOwnedEnds; }
            using ExtensionEndImpl = typename OwnedEnd::ManagedType;
            using ExtensionEndPtr = EGM::ManagedPtr<ExtensionEndImpl>;
            ExtensionEndPtr getOwnedEnd() { return m_extensionOwnedEnds.get(); }
            void setOwnedEnd(ExtensionEndImpl& end) { m_extensionOwnedEnds.set(end); }
            void setOwnedEnd(ExtensionEndPtr end) { m_extensionOwnedEnds.set(end); }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Extension> {
        static std::string name() { return "Extension"; }
        template <class Policy>
        static SetList sets(UML::Extension<Policy>& el) {
            return SetList {
                make_set_pair("ownedEnds", el.getOwnedEndSingleton())
            };
        }
    };
}

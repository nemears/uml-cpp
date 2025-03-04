#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

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
            using OwnedEnds = IndexableSet<ExtensionEnd, Extension>;
            OwnedEnds m_extensionOwnedEnds = OwnedEnds(this);
        private:
            void init() {
                m_extensionOwnedEnds.redefines(this->m_ownedEnds);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Extension);

            OwnedEnds& getOwnedEnds() { return m_extensionOwnedEnds; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Extension> {
        static std::string name() { return "Extension"; }
        template <class Policy>
        static SetList sets(UML::Extension<Policy>& el) {
            return SetList {
                make_set_pair("ownedEnds", el.getOwnedEnds())
            };
        }
    };
}

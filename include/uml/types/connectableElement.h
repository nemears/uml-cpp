#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class TypedElement;

    template <class ManagerPolicy>
    class ConnectableElement : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<ConnectableElement, EGM::TemplateTypeList<TypedElement>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(ConnectableElement);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::ConnectableElement> {
        static const bool abstract = true;
        static std::string name() { return "ConnectableElement"; }
    };
}

#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Class;

    template <class ManagerPolicy>
    class Stereotype : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Stereotype, EGM::TemplateTypeList<Class>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Stereotype);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Stereotype> {
        static std::string name() { return "Stereotype"; }
    };
}

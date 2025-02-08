#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Dependency;

    template <class ManagerPolicy>
    class Abstraction : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Abstraction, EGM::TemplateTypeList<Dependency>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Abstraction);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Abstraction> {
        static std::string name() { return "Abstraction"; }
    };
}

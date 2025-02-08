#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Abstraction;

    template <class ManagerPolicy>
    class Realization : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Realization, EGM::TemplateTypeList<Abstraction>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Realization);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Realization> {
        static std::string name() { return "Realization"; }
    };
}

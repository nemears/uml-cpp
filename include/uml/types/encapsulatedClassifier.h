#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class StructuredClassifier;

    template <class ManagerPolicy>
    class EncapsulatedClassifier : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<EncapsulatedClassifier, EGM::TemplateTypeList<StructuredClassifier>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(EncapsulatedClassifier);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::EncapsulatedClassifier> {
        static const bool abstract = true;
        static std::string name() { return "EncapsulatedClassifier"; }
    };
}

#pragma once

#include "egm/egm-basic.h"
#include "egm/manager/typeInfo.h"
#include "egm/util/templateTypeList.h"
#include "uml/types/packageableElement.h"

namespace UML {
    template <class>
    class PackageableElement;

    template <class ManagerPolicy>
    class Type : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Type, EGM::TemplateTypeList<PackageableElement>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Type);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Type> {
        static const bool abstract = true;
        static std::string name() { return "Type"; }
    };
}

#pragma once

#include "egm/egm-basic.h"
#include "egm/manager/typeInfo.h"
#include "egm/util/templateTypeList.h"

namespace UML {
    template <class>
    class DataType;

    template <class ManagerPolicy>
    class PrimitiveType: public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<PrimitiveType, EGM::TemplateTypeList<DataType>>;
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(PrimitiveType);
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::PrimitiveType> {
        static std::string name() { return "PrimitiveType"; }
    };
}

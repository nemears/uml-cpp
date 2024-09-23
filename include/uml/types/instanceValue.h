#pragma once

#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"
#include "valueSpecification.h"

namespace UML {

    class InstanceSpecification;
    typedef UmlPtr<InstanceSpecification> InstanceSpecificationPtr;

    class InstanceValue : public ValueSpecification {

        friend struct ElementInfo<InstanceValue>;

        protected:
            Singleton<InstanceSpecification, InstanceValue> m_instance = Singleton<InstanceSpecification, InstanceValue>(this);
            Singleton<InstanceSpecification, InstanceValue>& getInstanceSingleton();
        public:
            InstanceValue(std::size_t elementType, AbstractManager& manager);
            InstanceSpecificationPtr getInstance() const;
            void setInstance(InstanceSpecification& inst);
            void setInstance(InstanceSpecificationPtr inst);
            void setInstance(ID id);
            typedef TypeInfo<std::tuple<ValueSpecification>, InstanceValue> Info;
    };

    template <>
    struct ElementInfo<InstanceValue> : public DefaultInfo {
        static const bool abstract = false;
        static constexpr std::string_view name {"InstanceValue"};
        static SetList sets(InstanceValue& el) {
            return SetList {
                makeSetPair("instance", el.m_instance)
            };
        }
    };
}

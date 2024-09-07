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
            InstanceValue(std::size_t elementType, AbstractManager& manager);
        public:
            InstanceSpecificationPtr getInstance() const;
            void setInstance(InstanceSpecification& inst);
            void setInstance(InstanceSpecificationPtr inst);
            void setInstance(ID id);
            typedef TypeInfo<std::tuple<ValueSpecification>, InstanceValue> Info;
    };

    template <>
    struct ElementInfo<InstanceValue> {
        static const bool abstract = false;
        inline static const std::string name {"InstanceValue"};
        static SetList sets(InstanceValue& el) {
            return SetList {
                makeSetPair("instance", el.m_instance)
            };
        }
    };
}

#pragma once

#include "class.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {

    class BehavioralFeature;
    class Parameter;
    typedef UmlPtr<BehavioralFeature> BehavioralFeaturePtr;

    class Behavior : public Class {

        friend class BehavioralFeature;
        friend struct ElementInfo<Behavior>;

        protected:
            Set<Parameter, Behavior> m_ownedParameters = Set<Parameter, Behavior>(this);
            Singleton<BehavioralFeature, Behavior> m_specification = Singleton<BehavioralFeature, Behavior>(this);
            Singleton<BehavioralFeature, Behavior>& getSpecificationSingleton();
            Behavior(std::size_t elementType, AbstractManager& manager);
        public:
            Set<Parameter, Behavior>& getOwnedParameters();
            BehavioralFeaturePtr getSpecification() const;
            void setSpecification(BehavioralFeature& specification);
            void setSpecification(BehavioralFeaturePtr specification);
            void setSpecification(ID id);
            typedef TypeInfo<std::tuple<Class>, Behavior> Info;
    };

    template <>
    struct ElementInfo<Behavior> : public DefaultInfo {
        static constexpr std::string_view name {"Behavior"};
        static SetList sets(Behavior& el) {
            return SetList {
                makeSetPair("ownedParameters", el.m_ownedParameters),
                makeSetPair("specification", el.m_specification)
            };
        }
    };
}

#pragma once

#include "class.h"
#include "uml/set/singleton.h"

namespace UML {

    class BehavioralFeature;
    class Parameter;
    typedef UmlPtr<BehavioralFeature> BehavioralFeaturePtr;

    class Behavior : public Class {

        friend class BehavioralFeature;

        protected:
            Set<Parameter, Behavior> m_ownedParameters = Set<Parameter, Behavior>(this);
            Singleton<BehavioralFeature, Behavior> m_specification = Singleton<BehavioralFeature, Behavior>(this);
            void referenceErased(ID id) override;
            Singleton<BehavioralFeature, Behavior>& getSpecificationSingleton();
            Behavior();
        public:
            virtual ~Behavior();
            Set<Parameter, Behavior>& getOwnedParameters();
            BehavioralFeaturePtr getSpecification() const;
            void setSpecification(BehavioralFeature& specification);
            void setSpecification(BehavioralFeaturePtr specification);
            void setSpecification(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIOR;
            };
    };
}

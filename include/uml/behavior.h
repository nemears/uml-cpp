#ifndef _UML_BEHAVIOR_H_
#define _UML_BEHAVIOR_H_

#include "class.h"

namespace UML {

    class BehavioralFeature;
    typedef UmlPtr<BehavioralFeature> BehavioralFeaturePtr;

    class Behavior : public Class {

        friend class BehavioralFeature;

        protected:
            CustomSet<Parameter, Behavior> m_ownedParameters = CustomSet<Parameter, Behavior>(this);
            CustomSingleton<BehavioralFeature, Behavior> m_specification = CustomSingleton<BehavioralFeature, Behavior>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            TypedSet<BehavioralFeature, Behavior>& getSpecificationSingleton();
            void init();
            Behavior();
        public:
            virtual ~Behavior();
            Set<Parameter, Behavior>& getOwnedParameters();
            BehavioralFeaturePtr getSpecification() const;
            void setSpecification(BehavioralFeature& specification);
            void setSpecification(BehavioralFeature* specification);
            void setSpecification(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIOR;
            };
    };
}

#endif
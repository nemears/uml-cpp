#ifndef BEHAVIORH
#define BEHAVIORH

#include "class.h"

namespace UML {

    class BehavioralFeature;

    class Behavior : public Class {

        friend class UmlManager;
        friend class BehavioralFeature;

        protected:
            Set<Parameter, Behavior> m_ownedParameters = Set<Parameter, Behavior>(this);
            Singleton<BehavioralFeature, Behavior> m_specification = Singleton<BehavioralFeature, Behavior>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            Set<BehavioralFeature, Behavior>& getSpecificationSingleton();
            void init();
            void copy(const Behavior& rhs);
            Behavior();
        public:
            Behavior(const Behavior& rhs);
            virtual ~Behavior();
            Set<Parameter, Behavior>& getOwnedParameters();
            BehavioralFeature* getSpecification();
            BehavioralFeature& getSpecificationRef();
            ID getSpecificationID() const;
            bool hasSpecification() const;
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
#ifndef BEHAVIORH
#define BEHAVIORH

#include "class.h"

namespace UML {

    class BehavioralFeature;
    namespace Parsers {
        class BehaviorSetSpecification;
        class SetBehavioredClassifier;
    }

    class Behavior : public Class {

        friend class UmlManager;
        friend class BehavioralFeature;
        friend class Parsers::BehaviorSetSpecification;
        friend class Parsers::SetBehavioredClassifier;

        protected:
            Set<Parameter, Behavior> m_ownedParameters = Set<Parameter, Behavior>(this);
            Singleton<BehavioralFeature, Behavior> m_specification = Singleton<BehavioralFeature, Behavior>(this);
            // Singleton<BehavioredClassifier, Behavior> m_behavioredClassifier = Singleton<BehavioredClassifier, Behavior>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<BehavioralFeature, Behavior>& getSpecificationSingleton();
            // Set<BehavioredClassifier, Behavior>& getBehavioredClassifierSingleton();
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
            // BehavioredClassifier* getBehavioredClassifier();
            // BehavioredClassifier& getBehavioredClassifierRef();
            // ID getBehavioredClassifierID() const;
            // bool hasBehavioredClassifier() const;
            // void setBehavioredClassifier(BehavioredClassifier& classifier);
            // void setBehavioredClassifier(BehavioredClassifier* classifier);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIOR;
            };
    };
}

#endif
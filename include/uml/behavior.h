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
        friend class Parsers::BehaviorSetSpecification;
        friend class Parsers::SetBehavioredClassifier;

        protected:
            Sequence<Parameter> m_ownedParameters = Sequence<Parameter>(this);
            Singleton<BehavioralFeature, Behavior> m_specification = Singleton<BehavioralFeature, Behavior>(this);
            class RemoveSpecificationProcedure : public AbstractSingletonProcedure<BehavioralFeature, Behavior> {
                public:
                    RemoveSpecificationProcedure(Behavior* me) : AbstractSingletonProcedure<BehavioralFeature, Behavior>(me) {};
                    void operator()(BehavioralFeature* el) const override;
            };
            class AddSpecificationProcedure : public AbstractSingletonProcedure<BehavioralFeature, Behavior> {
                public:
                    AddSpecificationProcedure(Behavior* me) : AbstractSingletonProcedure<BehavioralFeature, Behavior>(me) {};
                    void operator()(BehavioralFeature* el) const override;
            };
            Singleton<BehavioredClassifier, Behavior> m_behavioredClassifier = Singleton<BehavioredClassifier, Behavior>(this);
            class RemoveBehavioredClassifierProcedure : public AbstractSingletonProcedure<BehavioredClassifier, Behavior> {
                public:
                    RemoveBehavioredClassifierProcedure(Behavior* me) : AbstractSingletonProcedure<BehavioredClassifier, Behavior>(me) {};
                    void operator()(BehavioredClassifier* el) const override;
            };
            class AddBehavioredClassifierProcedure : public AbstractSingletonProcedure<BehavioredClassifier, Behavior> {
                public:
                    AddBehavioredClassifierProcedure(Behavior* me) : AbstractSingletonProcedure<BehavioredClassifier, Behavior>(me) {};
                    void operator()(BehavioredClassifier* el) const override;
                    void operator()(ID id) const override;
            };
            class AddOwnedParameterFunctor : public TemplateAbstractSequenceFunctor<Parameter,Behavior> {
                public:
                    AddOwnedParameterFunctor(Behavior* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Parameter& el) const override;
            };
            class RemoveOwnedParameterFunctor : public TemplateAbstractSequenceFunctor<Parameter,Behavior> {
                public:
                    RemoveOwnedParameterFunctor(Behavior* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Parameter& el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Behavior();
        public:
            Behavior(const Behavior& rhs);
            virtual ~Behavior();
            Sequence<Parameter>& getOwnedParameters();
            BehavioralFeature* getSpecification();
            BehavioralFeature& getSpecificationRef();
            ID getSpecificationID() const;
            bool hasSpecification() const;
            void setSpecification(BehavioralFeature& specification);
            void setSpecification(BehavioralFeature* specification);
            BehavioredClassifier* getBehavioredClassifier();
            BehavioredClassifier& getBehavioredClassifierRef();
            ID getBehavioredClassifierID() const;
            bool hasBehavioredClassifier() const;
            void setBehavioredClassifier(BehavioredClassifier& classifier);
            void setBehavioredClassifier(BehavioredClassifier* classifier);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIOR;
            };
    };
}

#endif
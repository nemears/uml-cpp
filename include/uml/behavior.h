#ifndef BEHAVIORH
#define BEHAVIORH

#include "class.h"

namespace UML {

    class BehavioralFeature;

    class Behavior : public Class {
        protected:
            Sequence<Parameter> m_parameters;
            ID m_specificationID;
            BehavioralFeature* m_specificationPtr;
            ID m_behavioredClassifierID;
            BehavioredClassifier* m_behavioredClassifierPtr;
            class AddParameterFunctor : public TemplateAbstractSequenceFunctor<Parameter,Behavior> {
                public:
                    AddParameterFunctor(Behavior* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Parameter& el) const override;
            };
            class RemoveParameterFunctor : public TemplateAbstractSequenceFunctor<Parameter,Behavior> {
                public:
                    RemoveParameterFunctor(Behavior* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Parameter& el) const override;
            };
        public:
            Behavior();
            Behavior(const Behavior& rhs);
            virtual ~Behavior();
            Sequence<Parameter>& getParameters();
            BehavioralFeature* getSpecification();
            void setSpecification(BehavioralFeature* specification);
            BehavioredClassifier* getBehavioredClassifier();
            void setBehavioredClassifier(BehavioredClassifier* classifier);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIOR;
            };
    };
}

#endif
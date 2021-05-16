#ifndef BEHAVIORH
#define BEHAVIORH
#include "class.h"
#include "parameter.h"

namespace UML {

    class BehavioralFeature;

    class Behavior : public Class {
        protected:
            Sequence<Parameter> m_parameters;
            BehavioralFeature* m_specification;
            class AddParameterFunctor : public AbstractSequenceFunctor {
                public:
                    AddParameterFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveParameterFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveParameterFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Behavior();
            virtual ~Behavior();
            Sequence<Parameter>& getParameters();
            BehavioralFeature* getSpecification();
            void setSpecification(BehavioralFeature* specification);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
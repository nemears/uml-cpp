#ifndef BEHAVIORAL_FEATURE_H
#define BEHAVIORAL_FEATURE_H

#include "feature.h"
#include "namespace.h"
#include "behavior.h"
#include "parameter.h"

namespace UML {
    class BehavioralFeature : public Feature , public Namespace {
        protected:
            Sequence<Behavior>* m_methods;
            Sequence<Parameter>* m_ownedParameters;
            class AddMethodFunctor : public AbstractSequenceFunctor {
                public:
                    AddMethodFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddParameterFunctor : public AbstractSequenceFunctor {
                public:
                    AddParameterFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            BehavioralFeature();
            ~BehavioralFeature();
            Sequence<Behavior>& getMethods();
            Sequence<Parameter>& getOwnedParameters();
            bool isAbstract();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
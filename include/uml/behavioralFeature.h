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
            bool m_returnSpecified = false;
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
            class CheckParameterFunctor : public AbstractSequenceFunctor {
                public:
                    CheckParameterFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveParameterFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveParameterFunctor(Element* me) : AbstractSequenceFunctor(me) {};
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

    class ReturnParameterException : public std::exception {
        private:
            std::string m_msg;
        public:
            ReturnParameterException(const std::string elID) : m_msg(elID + " return parameter alreadu specified") {};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            };

    };
}

#endif
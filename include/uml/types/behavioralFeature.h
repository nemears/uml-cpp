#pragma once

#include "feature.h"
#include "namespace.h"

namespace UML {

    class Behavior;
    class Parameter;
    class ParameterSet;
    class Type;

    enum class CallConcurrencyKind {
        Sequential,
        Guarded,
        Concurrent
    };

    class BehavioralFeature : public Feature , public Namespace {
        protected:
            Set<Behavior, BehavioralFeature> m_methods = Set<Behavior, BehavioralFeature>(this);
            Set<Parameter, BehavioralFeature> m_ownedParameters = Set<Parameter, BehavioralFeature>(this);
            Set<Type, BehavioralFeature> m_raisedExceptions = Set<Type, BehavioralFeature>(this);
            Set<ParameterSet, BehavioralFeature> m_ownedParameterSets = Set<ParameterSet, BehavioralFeature>(this);
            bool m_returnSpecified = false;
            CallConcurrencyKind m_concurrency = CallConcurrencyKind::Sequential;
            void referenceErased(ID id) override;
            BehavioralFeature();
        public:
            virtual ~BehavioralFeature();
            Set<Behavior, BehavioralFeature>& getMethods();
            Set<Parameter, BehavioralFeature>& getOwnedParameters();
            Set<Type, BehavioralFeature>& getRaisedExceptions();
            Set<ParameterSet, BehavioralFeature>& getOwnedParameterSets();
            bool isAbstract();
            CallConcurrencyKind getConcurrency() const;
            void setConcurrency(CallConcurrencyKind concurrency);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::BEHAVIORAL_FEATURE;
            };
    };

    class ReturnParameterException : public std::exception {
        private:
            std::string m_msg;
        public:
            ReturnParameterException(const std::string elID) : m_msg(elID + " return parameter already specified") {};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            };

    };
}

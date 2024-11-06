#pragma once

#include "feature.h"
#include "namespace.h"
#include "uml/types/element.h"

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
            BehavioralFeature(std::size_t elementType, AbstractManager& manager);
        public:
            Set<Behavior, BehavioralFeature>& getMethods();
            Set<Parameter, BehavioralFeature>& getOwnedParameters();
            Set<Type, BehavioralFeature>& getRaisedExceptions();
            Set<ParameterSet, BehavioralFeature>& getOwnedParameterSets();
            bool isAbstract();
            CallConcurrencyKind getConcurrency() const;
            void setConcurrency(CallConcurrencyKind concurrency);
            typedef TypeInfo<std::tuple<Feature, Namespace>, BehavioralFeature> Info;
    };

    template <>
    struct ElementInfo<BehavioralFeature> : public DefaultInfo {
        static const bool abstract = true;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "BehavioralFeature"; }
        static SetList sets(BehavioralFeature& el) {
            return SetList {
                makeSetPair("methods", el.getMethods()),
                makeSetPair("ownedParameters", el.getOwnedParameters()),
                makeSetPair("raisedExceptions", el.getRaisedExceptions()),
                makeSetPair("ownedParameterSet", el.getOwnedParameterSets())
            };
        }
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

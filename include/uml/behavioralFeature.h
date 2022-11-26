#ifndef _UML_BEHAVIORAL_FEATURE_H_
#define _UML_BEHAVIORAL_FEATURE_H_

#include "feature.h"
#include "namespace.h"

namespace UML {

    class Behavior;
    class Parameter;
    class ParameterSet;

    enum class CallConcurrencyKind {
        Sequential,
        Guarded,
        Concurrent
    };

    class BehavioralFeature : public Feature , public Namespace {
        protected:
            CustomSet<Behavior, BehavioralFeature> m_methods = CustomSet<Behavior, BehavioralFeature>(this);
            CustomSet<Parameter, BehavioralFeature> m_ownedParameters = CustomSet<Parameter, BehavioralFeature>(this);
            CustomSet<Type, BehavioralFeature> m_raisedExceptions = CustomSet<Type, BehavioralFeature>(this);
            CustomSet<ParameterSet, BehavioralFeature> m_ownedParameterSets = CustomSet<ParameterSet, BehavioralFeature>(this);
            bool m_returnSpecified = false;
            CallConcurrencyKind m_concurrency = CallConcurrencyKind::Sequential;
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
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
            bool isSubClassOf(ElementType eType) const override;
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

#endif
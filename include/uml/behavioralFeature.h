#ifndef BEHAVIORAL_FEATURE_H
#define BEHAVIORAL_FEATURE_H

#include "feature.h"
#include "namespace.h"

namespace UML {
    class BehavioralFeature : public Feature , public Namespace {

        friend class UmlManager;

        protected:
            Set<Behavior, BehavioralFeature> m_methods = Set<Behavior, BehavioralFeature>(this);
            Set<Parameter, BehavioralFeature> m_ownedParameters = Set<Parameter, BehavioralFeature>(this);
            bool m_returnSpecified = false;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void init();
            void copy(const BehavioralFeature& rhs);
            BehavioralFeature();
        public:
            BehavioralFeature(const BehavioralFeature& el);
            virtual ~BehavioralFeature();
            Set<Behavior, BehavioralFeature>& getMethods();
            Set<Parameter, BehavioralFeature>& getOwnedParameters();
            bool isAbstract();
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
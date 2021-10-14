#ifndef BEHAVIORAL_FEATURE_H
#define BEHAVIORAL_FEATURE_H

#include "feature.h"
#include "namespace.h"

namespace UML {
    class BehavioralFeature : public Feature , public Namespace {

        friend class UmlManager;

        protected:
            Sequence<Behavior> m_methods = Sequence<Behavior>(this);
            Sequence<Parameter> m_ownedParameters = Sequence<Parameter>(this);
            bool m_returnSpecified = false;
            class AddMethodFunctor : public TemplateAbstractSequenceFunctor<Behavior,BehavioralFeature> {
                public:
                    AddMethodFunctor(BehavioralFeature* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Behavior& el) const override;
            };
            class RemoveMethodFunctor : public TemplateAbstractSequenceFunctor<Behavior,BehavioralFeature> {
                public:
                    RemoveMethodFunctor(BehavioralFeature* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Behavior& el) const override;
            };
            class AddParameterFunctor : public TemplateAbstractSequenceFunctor<Parameter,BehavioralFeature> {
                public:
                    AddParameterFunctor(BehavioralFeature* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Parameter& el) const override;
            };
            class CheckParameterFunctor : public TemplateAbstractSequenceFunctor<Parameter,BehavioralFeature> {
                public:
                    CheckParameterFunctor(BehavioralFeature* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Parameter& el) const override;
            };
            class RemoveParameterFunctor : public TemplateAbstractSequenceFunctor<Parameter,BehavioralFeature> {
                public:
                    RemoveParameterFunctor(BehavioralFeature* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Parameter& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            BehavioralFeature();
        public:
            BehavioralFeature(const BehavioralFeature& el);
            ~BehavioralFeature();
            Sequence<Behavior>& getMethods();
            Sequence<Parameter>& getOwnedParameters();
            bool isAbstract();
            ElementType getElementType() const override;
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
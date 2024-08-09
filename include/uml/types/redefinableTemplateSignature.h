#pragma once

#include "redefinableElement.h"
#include "templateSignature.h"

namespace UML {

    typedef UmlPtr<Classifier> ClassifierPtr;

    class RedefinableTemplateSignature : public RedefinableElement, public TemplateSignature {

        friend class Classifier;
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            struct ExtendedSignaturePolicy {
                void elementAdded(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me);
                void elementRemoved(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me);
            };
            struct ParameterPolicy {
                std::unordered_set<UmlPtr<RedefinableTemplateSignature>> addSignatures;
                std::unordered_set<UmlPtr<RedefinableTemplateSignature>> removeSignatures;
                void elementAdded(TemplateParameter& el, RedefinableTemplateSignature& me);
                void elementRemoved(TemplateParameter& el, RedefinableTemplateSignature& me);
            };
            Singleton<Classifier, RedefinableTemplateSignature> m_classifier = Singleton<Classifier, RedefinableTemplateSignature>(this);
            Set<RedefinableTemplateSignature, RedefinableTemplateSignature, ExtendedSignaturePolicy> m_extendedSignatures = Set<RedefinableTemplateSignature, RedefinableTemplateSignature, ExtendedSignaturePolicy>(this);
            Set<TemplateParameter, RedefinableTemplateSignature, ParameterPolicy> m_redefinableTemplateSignatureParameters = Set<TemplateParameter, RedefinableTemplateSignature, ParameterPolicy>(this);
            ReadOnlySet<TemplateParameter, RedefinableTemplateSignature> m_inheritedParameters = ReadOnlySet<TemplateParameter, RedefinableTemplateSignature>(this);
            Singleton<Classifier, RedefinableTemplateSignature>& getClassifierSingleton();
            void referenceErased(ID id) override;
            RedefinableTemplateSignature();
        public:
            virtual ~RedefinableTemplateSignature();
            ClassifierPtr getClassifier() const;
            void setClassifier(Classifier* classifier);
            void setClassifier(Classifier& classifier);
            void setClassifier(ClassifierPtr classifier);
            void setClassifier(ID id);
            Set<RedefinableTemplateSignature, RedefinableTemplateSignature, ExtendedSignaturePolicy>& getExtendedSignatures();
            ReadOnlySet<TemplateParameter, RedefinableTemplateSignature>& getInheritedParameters();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_TEMPLATE_SIGNATURE;
            }
    };
}

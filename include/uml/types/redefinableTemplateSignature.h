#pragma once

#include "redefinableElement.h"
#include "templateSignature.h"

namespace UML {

    typedef UmlPtr<Classifier> ClassifierPtr;

    class RedefinableTemplateSignature : public RedefinableElement, public TemplateSignature {

        friend class Classifier;
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            class AddExtendedSignaturePolicy {
                public:
                    void apply(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me);
            };
            class RemoveExtendedSignaturePolicy {
                public:
                    void apply(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me);
            };
            class AddParameterPolicy {
                public:
                    std::unordered_set<UmlPtr<RedefinableTemplateSignature>> addSignatures;
                    void apply(TemplateParameter& el, RedefinableTemplateSignature& me);
            };
            class RemoveParameterPolicy {
                public:
                    std::unordered_set<UmlPtr<RedefinableTemplateSignature>> removeSignatures;
                    void apply(TemplateParameter& el, RedefinableTemplateSignature& me);
            };
            CustomSingleton<Classifier, RedefinableTemplateSignature> m_classifier = CustomSingleton<Classifier, RedefinableTemplateSignature>(this);
            CustomSet<RedefinableTemplateSignature, RedefinableTemplateSignature, AddExtendedSignaturePolicy, RemoveExtendedSignaturePolicy> m_extendedSignatures = CustomSet<RedefinableTemplateSignature, RedefinableTemplateSignature, AddExtendedSignaturePolicy, RemoveExtendedSignaturePolicy>(this);
            CustomSet<TemplateParameter, RedefinableTemplateSignature, AddParameterPolicy, RemoveParameterPolicy> m_redefinableTemplateSignatureParameters = CustomSet<TemplateParameter, RedefinableTemplateSignature, AddParameterPolicy, RemoveParameterPolicy>(this);
            CustomReadOnlySet<TemplateParameter, RedefinableTemplateSignature> m_inheritedParameters = CustomReadOnlySet<TemplateParameter, RedefinableTemplateSignature>(this);
            TypedSet<Classifier, RedefinableTemplateSignature>& getClassifierSingleton();
            void referenceErased(ID id) override;
            RedefinableTemplateSignature();
        public:
            virtual ~RedefinableTemplateSignature();
            ClassifierPtr getClassifier() const;
            void setClassifier(Classifier* classifier);
            void setClassifier(Classifier& classifier);
            void setClassifier(ClassifierPtr classifier);
            void setClassifier(ID id);
            Set<RedefinableTemplateSignature, RedefinableTemplateSignature>& getExtendedSignatures();
            ReadOnlySet<TemplateParameter, RedefinableTemplateSignature>& getInheritedParameters();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_TEMPLATE_SIGNATURE;
            }
    };
}
#pragma once

#include "redefinableElement.h"
#include "templateSignature.h"
#include "uml/managers/typeInfo.h"
#include "uml/types/element.h"

namespace UML {

    typedef UmlPtr<Classifier> ClassifierPtr;

    class RedefinableTemplateSignature : public RedefinableElement, public TemplateSignature {

        friend class Classifier;
        friend struct ElementInfo<RedefinableTemplateSignature>;

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
            RedefinableTemplateSignature(std::size_t elementType, AbstractManager& manager);
        public:
            ClassifierPtr getClassifier() const;
            void setClassifier(Classifier* classifier);
            void setClassifier(Classifier& classifier);
            void setClassifier(ClassifierPtr classifier);
            void setClassifier(ID id);
            Set<RedefinableTemplateSignature, RedefinableTemplateSignature, ExtendedSignaturePolicy>& getExtendedSignatures();
            ReadOnlySet<TemplateParameter, RedefinableTemplateSignature>& getInheritedParameters();
            typedef TypeInfo<std::tuple<RedefinableElement, TemplateSignature>, RedefinableTemplateSignature> Info;
    };

    template <>
    struct ElementInfo<RedefinableTemplateSignature> {
        static const bool abstract = false;
        inline static const std::string name {"RedefinableTemplateSignature"};
        static SetList sets(RedefinableTemplateSignature& el) {
            return SetList {
                makeSetPair("classifier", el.m_classifier),
                makeSetPair("extendedSignatures", el.m_extendedSignatures),
                makeSetPair("parameters", el.m_redefinableTemplateSignatureParameters),
                makeSetPair("inheritedParameters", el.m_inheritedParameters)
            };
        }
    };
}

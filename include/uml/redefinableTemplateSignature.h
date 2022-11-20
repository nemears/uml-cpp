#ifndef _UML_REDEFINABLE_TEMPLATE_SIGNATURE_H_
#define _UML_REDEFINABLE_TEMPLATE_SIGNATURE_H_

#include "redefinableElement.h"
#include "templateSignature.h"

namespace UML {

    typedef UmlPtr<Classifier> ClassifierPtr;

    class RedefinableTemplateSignature : public RedefinableElement, public TemplateSignature {

        friend class Classifier;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            class AddExtendedSignaturePolicy {
                public:
                    static void apply(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me);
            };
            class RemoveExtendedSignaturePolicy {
                public:
                    static void apply(RedefinableTemplateSignature& el, RedefinableTemplateSignature& me);
            };
            CustomSingleton<Classifier, RedefinableTemplateSignature> m_classifier = CustomSingleton<Classifier, RedefinableTemplateSignature>(this);
            CustomSet<RedefinableTemplateSignature, RedefinableTemplateSignature, AddExtendedSignaturePolicy, RemoveExtendedSignaturePolicy> m_extendedSignatures = CustomSet<RedefinableTemplateSignature, RedefinableTemplateSignature, AddExtendedSignaturePolicy, RemoveExtendedSignaturePolicy>(this);
            CustomSet<TemplateParameter, RedefinableTemplateSignature> m_inheritedParameters = CustomSet<TemplateParameter, RedefinableTemplateSignature>(this);
            TypedSet<Classifier, RedefinableTemplateSignature>& getClassifierSingleton();
            void referenceReindexed(ID oldID, ID newID) override;
            void referenceErased(ID id) override;
            void init();
            RedefinableTemplateSignature();
        public:
            virtual ~RedefinableTemplateSignature();
            ClassifierPtr getClassifier() const;
            void setClassifier(Classifier* classifier);
            void setClassifier(Classifier& classifier);
            void setClassifier(ClassifierPtr classifier);
            void setClassifier(ID id);
            Set<RedefinableTemplateSignature, RedefinableTemplateSignature>& getExtendedSignatures();
            Set<TemplateParameter, RedefinableTemplateSignature>& getInheritedParameters();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_TEMPLATE_SIGNATURE;
            }
    };
}

#endif
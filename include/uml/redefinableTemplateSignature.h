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
            Singleton<Classifier, RedefinableTemplateSignature> m_classifier = Singleton<Classifier, RedefinableTemplateSignature>(this);
            Set<RedefinableTemplateSignature, RedefinableTemplateSignature> m_extendedSignatures = Set<RedefinableTemplateSignature, RedefinableTemplateSignature>(this);
            Set<TemplateParameter, RedefinableTemplateSignature> m_inheritedParameters = Set<TemplateParameter, RedefinableTemplateSignature>(this);
            class AddExtendedSignatureFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    AddExtendedSignatureFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveExtendedSignatureFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveExtendedSignatureFunctor(Element* el) : SetFunctor(el) {};
            };
            class AddParameterInExtendedSignatureFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RedefinableTemplateSignature* m_sig;
                    AddParameterInExtendedSignatureFunctor(Element* el) : SetFunctor(el) {};
            };
            class RemoveParameterInExtendedSignatureFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RedefinableTemplateSignature* m_sig;
                    RemoveParameterInExtendedSignatureFunctor(Element* el) : SetFunctor(el) {};
            };
            Set<Classifier, RedefinableTemplateSignature>& getClassifierSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
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
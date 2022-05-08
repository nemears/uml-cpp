#ifndef _UML_REDEFINABLE_TEMPLATE_SIGNATURE_H_
#define _UML_REDEFINABLE_TEMPLATE_SIGNATURE_H_

#include "redefinableElement.h"
#include "templateSignature.h"

namespace UML {

    typedef UmlPtr<Classifier> ClassifierPtr;

    class RedefinableTemplateSignature : public RedefinableElement, public TemplateSignature {

        friend class Classifier;
        friend class UmlManager;

        protected:
            Singleton<Classifier, RedefinableTemplateSignature> m_classifier = Singleton<Classifier, RedefinableTemplateSignature>(this);
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_TEMPLATE_SIGNATURE;
            }
    };
}

#endif
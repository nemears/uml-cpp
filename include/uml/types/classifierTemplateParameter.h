#ifndef _UML_CLASSIFIER_TEMPLATE_PARAMETER_H_
#define _UML_CLASSIFIER_TEMPLATE_PARAMETER_H_

#include "templateParameter.h"
#include "uml/set/set.h"

namespace UML {

    typedef UmlPtr<Classifier> ClassifierPtr;

    class ClassifierTemplateParameter : public TemplateParameter {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Classifier;

        protected:
            bool m_allowSubstitutable = true;
            CustomSingleton<Classifier, ClassifierTemplateParameter> m_classifierParameteredElement = CustomSingleton<Classifier, ClassifierTemplateParameter>(this);
            CustomSet<Classifier, ClassifierTemplateParameter> m_constrainingClassifiers = CustomSet<Classifier, ClassifierTemplateParameter>(this);
            TypedSet<Classifier, ClassifierTemplateParameter>& getParameteredElementSingleton();
            void referenceErased(ID id) override;
            ClassifierTemplateParameter();
        public:
            virtual ~ClassifierTemplateParameter();
            ClassifierPtr getParameteredElement() const;
            void setParameteredElement(Classifier* parameteredElement);
            void setParameteredElement(Classifier& parameteredElement);
            void setParameteredElement(ClassifierPtr parameteredElement);
            void setParameteredElement(ID id);
            Set<Classifier, ClassifierTemplateParameter>& getConstrainingClassifiers();
            bool isAllowSubstitutable() const;
            void setAllowSubstitutable(bool allowSubstitutable);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASSIFIER_TEMPLATE_PARAMETER;
            }
    };
}

#endif
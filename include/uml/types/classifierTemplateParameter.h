#pragma once

#include "templateParameter.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"

namespace UML {

    class Classifier;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class ClassifierTemplateParameter : public TemplateParameter {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Classifier;

        protected:
            bool m_allowSubstitutable = true;
            Singleton<Classifier, ClassifierTemplateParameter> m_classifierParameteredElement = Singleton<Classifier, ClassifierTemplateParameter>(this);
            Set<Classifier, ClassifierTemplateParameter> m_constrainingClassifiers = Set<Classifier, ClassifierTemplateParameter>(this);
            Singleton<Classifier, ClassifierTemplateParameter>& getParameteredElementSingleton();
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
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASSIFIER_TEMPLATE_PARAMETER;
            }
    };
}

#pragma once

#include "templateParameter.h"
#include "uml/managers/abstractManager.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {

    class Classifier;
    typedef UmlPtr<Classifier> ClassifierPtr;

    class ClassifierTemplateParameter : public TemplateParameter {

        friend class Classifier;
        friend struct ElementInfo<ClassifierTemplateParameter>;

        protected:
            bool m_allowSubstitutable = true;
            Singleton<Classifier, ClassifierTemplateParameter> m_classifierParameteredElement = Singleton<Classifier, ClassifierTemplateParameter>(this);
            Set<Classifier, ClassifierTemplateParameter> m_constrainingClassifiers = Set<Classifier, ClassifierTemplateParameter>(this);
            Singleton<Classifier, ClassifierTemplateParameter>& getParameteredElementSingleton();
        public:
            ClassifierTemplateParameter(std::size_t elementType, AbstractManager& manager);
            ClassifierPtr getParameteredElement() const;
            void setParameteredElement(Classifier* parameteredElement);
            void setParameteredElement(Classifier& parameteredElement);
            void setParameteredElement(ClassifierPtr parameteredElement);
            void setParameteredElement(ID id);
            Set<Classifier, ClassifierTemplateParameter>& getConstrainingClassifiers();
            bool isAllowSubstitutable() const;
            void setAllowSubstitutable(bool allowSubstitutable);
            typedef TypeInfo<std::tuple<TemplateParameter>, ClassifierTemplateParameter> Info;
    };

    template <>
    struct ElementInfo<ClassifierTemplateParameter> : public DefaultInfo {
        static const bool abstract = false;
        inline static const std::string name {"ClassifierTemplateParameter"};
        static SetList sets(ClassifierTemplateParameter& el) {
            return SetList {
                makeSetPair("parameteredElement", el.m_classifierParameteredElement),
                makeSetPair("constrainingClassifiers", el.m_constrainingClassifiers)
            };
        }
    };
}

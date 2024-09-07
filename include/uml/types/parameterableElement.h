#pragma once

#include "element.h"
#include "uml/set/singleton.h"

namespace UML {

    class TemplateParameter;
    class TemplateParameterSubstitution;

    typedef UmlPtr<TemplateParameter> TemplateParameterPtr;

    class ParameterableElement : virtual public Element {

        friend class TemplateParameter;
        friend class TemplateParameterSubstitution;
        friend struct ElementInfo<ParameterableElement>;

        protected:
            typedef TypeInfo<std::tuple<Element>, ParameterableElement> Info;
            Singleton<TemplateParameter, ParameterableElement> m_templateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            Singleton<TemplateParameter, ParameterableElement> m_owningTemplateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            Singleton<TemplateParameter, ParameterableElement>& getOwningTemplateParameterSingleton();
            Singleton<TemplateParameter, ParameterableElement>& getTemplateParameterSingleton();
            ParameterableElement();
        public:
            TemplateParameterPtr getOwningTemplateParameter() const;
            void setOwningTemplateParameter(TemplateParameterPtr parameter);
            void setOwningTemplateParameter(TemplateParameter& parameter);
            void setOwningTemplateParameter(ID id);
            TemplateParameterPtr getTemplateParameter() const;
            void setTemplateParameter(TemplateParameterPtr parameter);
            void setTemplateParameter(TemplateParameter& parameter);
            void setTemplateParameter(ID id);
    };

    template<>
    struct ElementInfo<ParameterableElement> {
        static SetList sets(ParameterableElement& el) {
            return SetList {
                makeSetPair("templateParameter", el.m_templateParameter),
                makeSetPair("owningTemplateParameter", el.m_owningTemplateParameter)
            };
        }
    };
}

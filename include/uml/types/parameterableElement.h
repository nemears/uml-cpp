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
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Singleton<TemplateParameter, ParameterableElement> m_templateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            Singleton<TemplateParameter, ParameterableElement> m_owningTemplateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            void referenceErased(ID id) override;
            Singleton<TemplateParameter, ParameterableElement>& getOwningTemplateParameterSingleton();
            Singleton<TemplateParameter, ParameterableElement>& getTemplateParameterSingleton();
            ParameterableElement();
        public:
            ~ParameterableElement();
            TemplateParameterPtr getOwningTemplateParameter() const;
            void setOwningTemplateParameter(TemplateParameter* parameter);
            void setOwningTemplateParameter(TemplateParameter& parameter);
            void setOwningTemplateParameter(ID id);
            TemplateParameterPtr getTemplateParameter() const;
            void setTemplateParameter(TemplateParameter* parameter);
            void setTemplateParameter(TemplateParameter& parameter);
            void setTemplateParameter(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETERABLE_ELEMENT;
            };
    };
}

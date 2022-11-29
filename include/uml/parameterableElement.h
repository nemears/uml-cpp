#ifndef _UML_PARAMETERABLE_ELEMENT_H_
#define _UML_PARAMETERABLE_ELEMENT_H_

#include "element.h"
#include "set/singleton.h"

namespace UML {

    class TemplateParameter;
    class TemplateParameterSubstitution;

    typedef UmlPtr<TemplateParameter> TemplateParameterPtr;

    class ParameterableElement : virtual public Element {

        friend class TemplateParameter;
        friend class TemplateParameterSubstitution;
        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSingleton<TemplateParameter, ParameterableElement> m_templateParameter = CustomSingleton<TemplateParameter, ParameterableElement>(this);
            CustomSingleton<TemplateParameter, ParameterableElement> m_owningTemplateParameter = CustomSingleton<TemplateParameter, ParameterableElement>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            TypedSet<TemplateParameter, ParameterableElement>& getOwningTemplateParameterSingleton();
            TypedSet<TemplateParameter, ParameterableElement>& getTemplateParameterSingleton();
            void init();
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

#endif
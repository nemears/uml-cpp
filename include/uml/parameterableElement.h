#ifndef PARAMETERABLE_ELEMENT_H
#define PARAMETERABLE_ELEMENT_H

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateParameter;
    class TemplateParameterSubstitution;

    class ParameterableElement : virtual public Element {

        friend class TemplateParameter;
        friend class TemplateParameterSubstitution;
        friend class UmlManager;

        protected:
            Singleton<TemplateParameter, ParameterableElement> m_templateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            Singleton<TemplateParameter, ParameterableElement> m_owningTemplateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            Set<TemplateParameter, ParameterableElement>& getOwningTemplateParameterSingleton();
            Set<TemplateParameter, ParameterableElement>& getTemplateParameterSingleton();
            void init();
            void copy(const ParameterableElement& rhs);
            ParameterableElement();
        public:
            ParameterableElement(const ParameterableElement& el);
            ~ParameterableElement();
            TemplateParameter* getOwningTemplateParameter();
            TemplateParameter& getOwningTemplateParameterRef();
            ID getOwningTemplateParameterID() const;
            bool hasOwningTemplateParameter()  const;
            void setOwningTemplateParameter(TemplateParameter* parameter);
            void setOwningTemplateParameter(TemplateParameter& parameter);
            void setOwningTemplateParameter(ID id);
            TemplateParameter* getTemplateParameter();
            TemplateParameter& getTemplateParameterRef();
            ID getTemplateParameterID() const;
            bool hasTemplateParameter() const;
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
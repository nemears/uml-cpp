#ifndef PARAMETERABLE_ELEMENT_H
#define PARAMETERABLE_ELEMENT_H

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateParameter;
    class TemplateParameterSubstitution;
    namespace Parsers {
        class SetOwningTemplateParameter;
    }

    class ParameterableElement : virtual public Element {

        friend class TemplateParameter;
        friend class TemplateParameterSubstitution;
        friend class UmlManager;
        friend class Parsers::SetOwningTemplateParameter;

        protected:
            Singleton<TemplateParameter, ParameterableElement> m_owningTemplateParameter = Singleton<TemplateParameter, ParameterableElement>(this);
            class RemoveOwningTemplateParameterProcedure : public AbstractSingletonProcedure<TemplateParameter, ParameterableElement> {
                public:
                    RemoveOwningTemplateParameterProcedure(ParameterableElement* me) : AbstractSingletonProcedure<TemplateParameter, ParameterableElement>(me) {};
                    void operator()(TemplateParameter* el) const override;
            };
            class AddOwningTemplateParameterProcedure : public AbstractSingletonProcedure<TemplateParameter, ParameterableElement> {
                public:
                    AddOwningTemplateParameterProcedure(ParameterableElement* me) : AbstractSingletonProcedure<TemplateParameter, ParameterableElement>(me) {};
                    void operator()(TemplateParameter* el) const override;
                    void operator()(ID id) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
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
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PARAMETERABLE_ELEMENT;
            };
    };
}

#endif
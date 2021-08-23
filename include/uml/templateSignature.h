#ifndef TEMPLATE_SIGNATURE_H
#define TEMPLATE_SIGNATURE_H

#include "element.h"
#include "sequence.h"

namespace UML {

    class TemplateableElement;
    class TemplateParameter;

    class TemplateSignature : public Element {
        friend class UmlManager;
        private:
            ID m_templateID;
            TemplateableElement* m_templatePtr;
            Sequence<TemplateParameter> m_ownedParameter;
            Sequence<TemplateParameter> m_parameter;

            class AddOwnedParameterFunctor : public TemplateAbstractSequenceFunctor<TemplateParameter,TemplateSignature> {
                public:
                    AddOwnedParameterFunctor(TemplateSignature* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(TemplateParameter& el) const override;
            };
            class RemoveOwnedParameterFunctor : public TemplateAbstractSequenceFunctor<TemplateParameter,TemplateSignature> {
                public:
                    RemoveOwnedParameterFunctor(TemplateSignature* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(TemplateParameter& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            TemplateSignature();
            TemplateSignature(const TemplateSignature& el);
            virtual ~TemplateSignature();
            TemplateableElement* getTemplate();
            void setTemplate(TemplateableElement* temp);
            Sequence<TemplateParameter>& getOwnedParameter();
            Sequence<TemplateParameter>& getParameter();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_SIGNATURE;
            };
    };
}

#endif
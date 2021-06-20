#ifndef TEMPLATE_SIGNATURE_H
#define TEMPLATE_SIGNATURE_H

#include "element.h"
#include "templateParameter.h"
#include "sequence.h"

namespace UML {

    class TemplateableElement;

    class TemplateSignature : public Element {
        friend class UmlManager;
        private:
            ID m_templateID;
            TemplateableElement* m_templatePtr;
            Sequence<TemplateParameter> m_ownedParameter;

            class AddOwnedParameterFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedParameterFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedParameterFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedParameterFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            TemplateSignature();
            TemplateSignature(const TemplateSignature& el);
            virtual ~TemplateSignature();
            TemplateableElement* getTemplate();
            void setTemplate(TemplateableElement* temp);
            Sequence<TemplateParameter>& getOwnedParameter();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_SIGNATURE;
            };
    };
}

#endif
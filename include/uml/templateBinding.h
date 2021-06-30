#ifndef TEMPLATE_BINDING_H
#define TEMPLATE_BINDING_H

#include "directedRelationship.h"
#include "templateableElement.h"
#include "templateParameterSubstitution.h"

namespace UML {
    class TemplateBinding : public DirectedRelationship {
        friend class UmlManager;
        private:
            ID m_boundElementID;
            TemplateableElement* m_boundElementPtr;
            ID m_signatureID;
            TemplateSignature* m_signaturePtr;
            Sequence<TemplateParameterSubstitution> m_parameterSubstitution;

            class AddParameterSubstitutionFunctor : public AbstractSequenceFunctor {
                public:
                    AddParameterSubstitutionFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveParameterSubstitutionFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveParameterSubstitutionFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            TemplateBinding();
            TemplateBinding(const TemplateBinding& bind);
            ~TemplateBinding();
            TemplateableElement* getBoundElement(); 
            void setBoundElement(TemplateableElement* el);
            TemplateSignature* getSignature();
            void setSignature(TemplateSignature* signature);
            Sequence<TemplateParameterSubstitution>& getParameterSubstitution();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_BINDING;
            };
    };
}

#endif
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
            Singleton<TemplateableElement, TemplateSignature> m_template = Singleton<TemplateableElement, TemplateSignature>(this);
            bool m_setFlag = false;
            class RemoveTemplateProcedure : public AbstractSingletonProcedure<TemplateableElement, TemplateSignature> {
                public:
                    RemoveTemplateProcedure(TemplateSignature* me) : AbstractSingletonProcedure<TemplateableElement, TemplateSignature>(me) {};
                    void operator()(TemplateableElement* el) const override;
            };
            class AddTemplateProcedure : public AbstractSingletonProcedure<TemplateableElement, TemplateSignature> {
                public:
                    AddTemplateProcedure(TemplateSignature* me) : AbstractSingletonProcedure<TemplateableElement, TemplateSignature>(me) {};
                    void operator()(TemplateableElement* el) const override;
            };
            Sequence<TemplateParameter> m_ownedParameter =  Sequence<TemplateParameter>(this);
            Sequence<TemplateParameter> m_parameter = Sequence<TemplateParameter>(this);

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
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            TemplateSignature();
        public:
            TemplateSignature(const TemplateSignature& el);
            virtual ~TemplateSignature();
            TemplateableElement* getTemplate();
            TemplateableElement& getTemplateRef();
            bool hasTemplate() const;
            void setTemplate(TemplateableElement& temp);
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
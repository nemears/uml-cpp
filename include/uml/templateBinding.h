#ifndef TEMPLATE_BINDING_H
#define TEMPLATE_BINDING_H

#include "directedRelationship.h"

namespace UML {

    class TemplateParameterSubstitution;
    class TemplateableElement;

    class TemplateBinding : public DirectedRelationship {

        friend class UmlManager;

        private:
            Singleton<TemplateableElement, TemplateBinding> m_boundElement = Singleton<TemplateableElement, TemplateBinding>(this);
            bool m_setFlag = false;
            class RemoveBoundElementProcedure : public AbstractSingletonProcedure<TemplateableElement, TemplateBinding> {
                public:
                    RemoveBoundElementProcedure(TemplateBinding* me) : AbstractSingletonProcedure<TemplateableElement, TemplateBinding>(me) {};
                    void operator()(TemplateableElement* el) const override;
            };
            class AddBoundElementProcedure : public AbstractSingletonProcedure<TemplateableElement, TemplateBinding> {
                public:
                    AddBoundElementProcedure(TemplateBinding* me) : AbstractSingletonProcedure<TemplateableElement, TemplateBinding>(me) {};
                    void operator()(TemplateableElement* el) const override;
            };
            Singleton<TemplateSignature, TemplateBinding> m_signature = Singleton<TemplateSignature, TemplateBinding>(this);
            class RemoveSignatureProcedure : public AbstractSingletonProcedure<TemplateSignature, TemplateBinding> {
                public:
                    RemoveSignatureProcedure(TemplateBinding* me) : AbstractSingletonProcedure<TemplateSignature, TemplateBinding>(me) {};
                    void operator()(TemplateSignature* el) const override;
            };
            class AddSignatureProcedure : public AbstractSingletonProcedure<TemplateSignature, TemplateBinding> {
                public:
                    AddSignatureProcedure(TemplateBinding* me) : AbstractSingletonProcedure<TemplateSignature, TemplateBinding>(me) {};
                    void operator()(TemplateSignature* el) const override;
            };
            Sequence<TemplateParameterSubstitution> m_parameterSubstitution = Sequence<TemplateParameterSubstitution>(this);

            class AddParameterSubstitutionFunctor : public TemplateAbstractSequenceFunctor<TemplateParameterSubstitution,TemplateBinding> {
                public:
                    AddParameterSubstitutionFunctor(TemplateBinding* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(TemplateParameterSubstitution& el) const override;
            };
            class RemoveParameterSubstitutionFunctor : public TemplateAbstractSequenceFunctor<TemplateParameterSubstitution,TemplateBinding> {
                public:
                    RemoveParameterSubstitutionFunctor(TemplateBinding* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(TemplateParameterSubstitution& el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            TemplateBinding();
        public:
            TemplateBinding(const TemplateBinding& bind);
            ~TemplateBinding();
            TemplateableElement* getBoundElement(); 
            TemplateableElement& getBoundElementRef();
            bool hasBoundElement() const;
            void setBoundElement(TemplateableElement& el);
            void setBoundElement(TemplateableElement* el);
            TemplateSignature* getSignature();
            TemplateSignature& getSignatureRef();
            bool hasSignature() const;
            void setSignature(TemplateSignature& signature);
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
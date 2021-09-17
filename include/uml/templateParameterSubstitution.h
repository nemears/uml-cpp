#ifndef TEMPLATE_PARAMETER_SUBSTITUTION_H
#define TEMPLATE_PARAMETER_SUBSTITUTION_H

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateBinding;
    class TemplateParameter;
    class ParameterableElement;

    class TemplateParameterSubstitution : public Element {

        friend class UmlManager;

        private:
            Singleton<TemplateParameter, TemplateParameterSubstitution> m_formal = Singleton<TemplateParameter, TemplateParameterSubstitution>(this);
            class RemoveFormalProcedure : public AbstractSingletonProcedure<TemplateParameter, TemplateParameterSubstitution> {
                public:
                    RemoveFormalProcedure(TemplateParameterSubstitution* me) : AbstractSingletonProcedure<TemplateParameter, TemplateParameterSubstitution>(me) {};
                    void operator()(TemplateParameter* el) const override;
            };
            class AddFormalProcedure : public AbstractSingletonProcedure<TemplateParameter, TemplateParameterSubstitution> {
                public:
                    AddFormalProcedure(TemplateParameterSubstitution* me) : AbstractSingletonProcedure<TemplateParameter, TemplateParameterSubstitution>(me) {};
                    void operator()(TemplateParameter* el) const override;
            };
            Singleton<TemplateBinding, TemplateParameterSubstitution> m_templateBinding = Singleton<TemplateBinding, TemplateParameterSubstitution>(this);
            class RemoveTemplateBindingProcedure : public AbstractSingletonProcedure<TemplateBinding, TemplateParameterSubstitution> {
                public:
                    RemoveTemplateBindingProcedure(TemplateParameterSubstitution* me) : AbstractSingletonProcedure<TemplateBinding, TemplateParameterSubstitution>(me) {};
                    void operator()(TemplateBinding* el) const override;
            };
            class AddTemplateBindingProcedure : public AbstractSingletonProcedure<TemplateBinding, TemplateParameterSubstitution> {
                public:
                    AddTemplateBindingProcedure(TemplateParameterSubstitution* me) : AbstractSingletonProcedure<TemplateBinding, TemplateParameterSubstitution>(me) {};
                    void operator()(TemplateBinding* el) const override;
            };
            Singleton<ParameterableElement, TemplateParameterSubstitution> m_actual = Singleton<ParameterableElement, TemplateParameterSubstitution>(this);
            class RemoveActualProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameterSubstitution> {
                public:
                    RemoveActualProcedure(TemplateParameterSubstitution* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameterSubstitution>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            class AddActualProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameterSubstitution> {
                public:
                    AddActualProcedure(TemplateParameterSubstitution* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameterSubstitution>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            Singleton<ParameterableElement, TemplateParameterSubstitution> m_ownedActual = Singleton<ParameterableElement, TemplateParameterSubstitution>(this);
            class RemoveOwnedActualProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameterSubstitution>{
                public:
                    RemoveOwnedActualProcedure(TemplateParameterSubstitution * me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameterSubstitution>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            class AddOwnedActualProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameterSubstitution> {
                public:
                    AddOwnedActualProcedure(TemplateParameterSubstitution* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameterSubstitution>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            TemplateParameterSubstitution();
        public:
            TemplateParameterSubstitution(const TemplateParameterSubstitution& sub);
            ~TemplateParameterSubstitution();
            TemplateParameter* getFormal();
            TemplateParameter& getFormalRef();
            bool hasFormal() const;
            void setFormal(TemplateParameter& formal);
            void setFormal(TemplateParameter* formal);
            TemplateBinding* getTemplateBinding();
            TemplateBinding& getTemplateBindingRef();
            bool hasTemplateBinding() const;
            void setTemplateBinding(TemplateBinding& binding);
            void setTemplateBinding(TemplateBinding* binding);
            ParameterableElement* getActual();
            ParameterableElement& getActualRef();
            bool hasActual() const;
            void setActual(ParameterableElement& actual);
            void setActual(ParameterableElement* actual);
            ParameterableElement* getOwnedActual();
            ParameterableElement& getOwnedActualRef();
            bool hasOwnedActual() const;
            void setOwnedActual(ParameterableElement& actual);
            void setOwnedActual(ParameterableElement* actual);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
            };
    };
}

#endif
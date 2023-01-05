#ifndef _UML_TEMPLATE_PARAMETER_SUBSTITUTION_H_
#define _UML_TEMPLATE_PARAMETER_SUBSTITUTION_H_

#include "element.h"
#include "set/singleton.h"

namespace UML {

    class TemplateBinding;
    class TemplateParameter;
    class ParameterableElement;

    typedef UmlPtr<TemplateParameter> TemplateParameterPtr;
    typedef UmlPtr<ParameterableElement> ParameterableElementPtr;
    typedef UmlPtr<TemplateBinding> TemplateBindingPtr;

    class TemplateParameterSubstitution : public Element {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class TemplateBinding;

        private:
            CustomSingleton<TemplateParameter, TemplateParameterSubstitution> m_formal = CustomSingleton<TemplateParameter, TemplateParameterSubstitution>(this);
            CustomSingleton<TemplateBinding, TemplateParameterSubstitution> m_templateBinding = CustomSingleton<TemplateBinding, TemplateParameterSubstitution>(this);
            CustomSingleton<ParameterableElement, TemplateParameterSubstitution> m_actual = CustomSingleton<ParameterableElement, TemplateParameterSubstitution>(this);
            CustomSingleton<ParameterableElement, TemplateParameterSubstitution> m_ownedActual = CustomSingleton<ParameterableElement, TemplateParameterSubstitution>(this);
            void referenceReindexed(ID newID) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            TypedSet<TemplateParameter, TemplateParameterSubstitution>& getFormalSingleton();
            TypedSet<TemplateBinding, TemplateParameterSubstitution>& getTemplateBindingSingleton();
            TypedSet<ParameterableElement, TemplateParameterSubstitution>& getActualSingleton();
            TypedSet<ParameterableElement, TemplateParameterSubstitution>& getOwnedActualSingleton();
            TemplateParameterSubstitution();
        public:
            ~TemplateParameterSubstitution();
            TemplateParameterPtr getFormal() const;
            void setFormal(TemplateParameter& formal);
            void setFormal(TemplateParameter* formal);
            void setFormal(ID id);
            TemplateBindingPtr getTemplateBinding() const;
            void setTemplateBinding(TemplateBinding& binding);
            void setTemplateBinding(TemplateBinding* binding);
            void setTemplateBinding(ID id);
            ParameterableElementPtr getActual() const;
            void setActual(ParameterableElement& actual);
            void setActual(ParameterableElement* actual);
            void setActual(ID id);
            ParameterableElementPtr getOwnedActual() const;
            void setOwnedActual(ParameterableElement& actual);
            void setOwnedActual(ParameterableElement* actual);
            void setOwnedActual(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
            };
    };
}

#endif
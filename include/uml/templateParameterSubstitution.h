#ifndef TEMPLATE_PARAMETER_SUBSTITUTION_H
#define TEMPLATE_PARAMETER_SUBSTITUTION_H

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateBinding;
    class TemplateParameter;
    class ParameterableElement;
    namespace Parsers {
        class SetOwnedActual;
        class SetActual;
        class SetFormal;
        class SetTemplateBinding;
    }

    class TemplateParameterSubstitution : public Element {

        friend class UmlManager;
        friend class Parsers::SetOwnedActual;
        friend class Parsers::SetActual;
        friend class Parsers::SetFormal;
        friend class Parsers::SetTemplateBinding;

        private:
            Singleton<TemplateParameter, TemplateParameterSubstitution> m_formal = Singleton<TemplateParameter, TemplateParameterSubstitution>(this);
            Singleton<TemplateBinding, TemplateParameterSubstitution> m_templateBinding = Singleton<TemplateBinding, TemplateParameterSubstitution>(this);
            Singleton<ParameterableElement, TemplateParameterSubstitution> m_actual = Singleton<ParameterableElement, TemplateParameterSubstitution>(this);
            Singleton<ParameterableElement, TemplateParameterSubstitution> m_ownedActual = Singleton<ParameterableElement, TemplateParameterSubstitution>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<TemplateParameter, TemplateParameterSubstitution>& getFormalSingleton();
            Set<TemplateBinding, TemplateParameterSubstitution>& getTemplateBindingSingleton();
            Set<ParameterableElement, TemplateParameterSubstitution>& getActualSingleton();
            Set<ParameterableElement, TemplateParameterSubstitution>& getOwnedActualSingleton();
            void init();
            void copy(const TemplateParameterSubstitution& rhs);
            TemplateParameterSubstitution();
        public:
            TemplateParameterSubstitution(const TemplateParameterSubstitution& sub);
            ~TemplateParameterSubstitution();
            TemplateParameter* getFormal();
            TemplateParameter& getFormalRef();
            ID getFormalID() const;
            bool hasFormal() const;
            void setFormal(TemplateParameter& formal);
            void setFormal(TemplateParameter* formal);
            TemplateBinding* getTemplateBinding();
            TemplateBinding& getTemplateBindingRef();
            ID getTemplateBindingID() const;
            bool hasTemplateBinding() const;
            void setTemplateBinding(TemplateBinding& binding);
            void setTemplateBinding(TemplateBinding* binding);
            ParameterableElement* getActual();
            ParameterableElement& getActualRef();
            ID getActualID() const;
            bool hasActual() const;
            void setActual(ParameterableElement& actual);
            void setActual(ParameterableElement* actual);
            ParameterableElement* getOwnedActual();
            ParameterableElement& getOwnedActualRef();
            ID getOwnedActualID() const;
            bool hasOwnedActual() const;
            void setOwnedActual(ParameterableElement& actual);
            void setOwnedActual(ParameterableElement* actual);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER_SUBSTITUTION;
            };
    };
}

#endif
#ifndef _UML_TEMPLATE_PARAMETER_SUBSTITUTION_H_
#define _UML_TEMPLATE_PARAMETER_SUBSTITUTION_H_

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateBinding;
    class TemplateParameter;
    class ParameterableElement;

    typedef UmlPtr<TemplateParameter> TemplateParameterPtr;
    typedef UmlPtr<ParameterableElement> ParameterableElementPtr;
    typedef UmlPtr<TemplateBinding> TemplateBindingPtr;

    class TemplateParameterSubstitution : public Element {

        friend class UmlManager;
        friend class TemplateBinding;

        private:
            Singleton<TemplateParameter, TemplateParameterSubstitution> m_formal = Singleton<TemplateParameter, TemplateParameterSubstitution>(this);
            Singleton<TemplateBinding, TemplateParameterSubstitution> m_templateBinding = Singleton<TemplateBinding, TemplateParameterSubstitution>(this);
            Singleton<ParameterableElement, TemplateParameterSubstitution> m_actual = Singleton<ParameterableElement, TemplateParameterSubstitution>(this);
            Singleton<ParameterableElement, TemplateParameterSubstitution> m_ownedActual = Singleton<ParameterableElement, TemplateParameterSubstitution>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            Set<TemplateParameter, TemplateParameterSubstitution>& getFormalSingleton();
            Set<TemplateBinding, TemplateParameterSubstitution>& getTemplateBindingSingleton();
            Set<ParameterableElement, TemplateParameterSubstitution>& getActualSingleton();
            Set<ParameterableElement, TemplateParameterSubstitution>& getOwnedActualSingleton();
            void init();
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
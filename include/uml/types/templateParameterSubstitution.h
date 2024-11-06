#pragma once

#include "element.h"
#include "uml/set/singleton.h"

namespace UML {

    class TemplateBinding;
    class TemplateParameter;
    class ParameterableElement;

    typedef UmlPtr<TemplateParameter> TemplateParameterPtr;
    typedef UmlPtr<ParameterableElement> ParameterableElementPtr;
    typedef UmlPtr<TemplateBinding> TemplateBindingPtr;

    class TemplateParameterSubstitution : public Element {

        friend class TemplateBinding;
        friend struct ElementInfo<TemplateParameterSubstitution>;

        protected:
            Singleton<TemplateParameter, TemplateParameterSubstitution> m_formal = Singleton<TemplateParameter, TemplateParameterSubstitution>(this);
            Singleton<TemplateBinding, TemplateParameterSubstitution> m_templateBinding = Singleton<TemplateBinding, TemplateParameterSubstitution>(this);
            Singleton<ParameterableElement, TemplateParameterSubstitution> m_actual = Singleton<ParameterableElement, TemplateParameterSubstitution>(this);
            Singleton<ParameterableElement, TemplateParameterSubstitution> m_ownedActual = Singleton<ParameterableElement, TemplateParameterSubstitution>(this);
            Singleton<TemplateParameter, TemplateParameterSubstitution>& getFormalSingleton();
            Singleton<TemplateBinding, TemplateParameterSubstitution>& getTemplateBindingSingleton();
            Singleton<ParameterableElement, TemplateParameterSubstitution>& getActualSingleton();
            Singleton<ParameterableElement, TemplateParameterSubstitution>& getOwnedActualSingleton();
        public:
            TemplateParameterSubstitution(std::size_t elementType, AbstractManager& manager);
            TemplateParameterPtr getFormal() const;
            void setFormal(TemplateParameter& formal);
            void setFormal(TemplateParameterPtr formal);
            void setFormal(ID id);
            TemplateBindingPtr getTemplateBinding() const;
            void setTemplateBinding(TemplateBinding& binding);
            void setTemplateBinding(TemplateBindingPtr binding);
            void setTemplateBinding(ID id);
            ParameterableElementPtr getActual() const;
            void setActual(ParameterableElement& actual);
            void setActual(ParameterableElementPtr actual);
            void setActual(ID id);
            ParameterableElementPtr getOwnedActual() const;
            void setOwnedActual(ParameterableElement& actual);
            void setOwnedActual(ParameterableElementPtr actual);
            void setOwnedActual(ID id);
            typedef TypeInfo<std::tuple<Element>, TemplateParameterSubstitution> Info;
    };

    template <>
    struct ElementInfo<TemplateParameterSubstitution> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "TemplateParameterSubstitution"; }
        static SetList sets(TemplateParameterSubstitution& el) {
            return SetList {
                makeSetPair("formal", el.m_formal),
                makeSetPair("binding", el.m_templateBinding),
                makeSetPair("actual", el.m_actual),
                makeSetPair("ownedActual", el.m_ownedActual)
            };
        }
    };
}

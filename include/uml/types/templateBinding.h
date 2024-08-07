#pragma once

#include "directedRelationship.h"
#include "uml/set/singleton.h"

namespace UML {

    class TemplateParameterSubstitution;
    class TemplateableElement;
    class TemplateSignature;

    typedef UmlPtr<TemplateableElement> TemplateableElementPtr;
    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;

    class TemplateBinding : public DirectedRelationship {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class TemplateableElement;
        
        private:
            Singleton<TemplateableElement, TemplateBinding> m_boundElement = Singleton<TemplateableElement, TemplateBinding>(this);
            Singleton<TemplateSignature, TemplateBinding> m_signature = Singleton<TemplateSignature, TemplateBinding>(this);
            Set<TemplateParameterSubstitution, TemplateBinding> m_parameterSubstitutions = Set<TemplateParameterSubstitution, TemplateBinding>(this);
            Singleton<TemplateableElement, TemplateBinding>& getBoundElementSingleton();
            Singleton<TemplateSignature, TemplateBinding>& getSignatureSingleton();
            TemplateBinding();
        public:
            ~TemplateBinding();
            TemplateableElementPtr getBoundElement() const;
            void setBoundElement(TemplateableElement& el);
            void setBoundElement(TemplateableElement* el);
            void setBoundElement(ID id);
            TemplateSignaturePtr getSignature() const;
            void setSignature(TemplateSignature& signature);
            void setSignature(TemplateSignature* signature);
            void setSignature(ID id);
            Set<TemplateParameterSubstitution, TemplateBinding>& getParameterSubstitutions();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_BINDING;
            };
    };
}

#pragma once

#include "directedRelationship.h"
#include "templateParameterSubstitution.h"
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
            CustomSingleton<TemplateableElement, TemplateBinding> m_boundElement = CustomSingleton<TemplateableElement, TemplateBinding>(this);
            CustomSingleton<TemplateSignature, TemplateBinding> m_signature = CustomSingleton<TemplateSignature, TemplateBinding>(this);
            CustomSet<TemplateParameterSubstitution, TemplateBinding> m_parameterSubstitutions = CustomSet<TemplateParameterSubstitution, TemplateBinding>(this);
            TypedSet<TemplateableElement, TemplateBinding>& getBoundElementSingleton();
            TypedSet<TemplateSignature, TemplateBinding>& getSignatureSingleton();
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_BINDING;
            };
    };
}
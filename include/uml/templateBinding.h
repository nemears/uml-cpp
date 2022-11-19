#ifndef _UML_TEMPLATE_BINDING_H_
#define _UML_TEMPLATE_BINDING_H_

#include "directedRelationship.h"
#include "templateParameterSubstitution.h"
#include "set/singleton.h"

namespace UML {

    class TemplateParameterSubstitution;
    class TemplateableElement;
    class TemplateSignature;

    typedef UmlPtr<TemplateableElement> TemplateableElementPtr;
    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;

    class TemplateBinding : public DirectedRelationship {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class TemplateableElement;
        
        private:
            CustomSingleton<TemplateableElement, TemplateBinding> m_boundElement = CustomSingleton<TemplateableElement, TemplateBinding>(this);
            CustomSingleton<TemplateSignature, TemplateBinding> m_signature = CustomSingleton<TemplateSignature, TemplateBinding>(this);
            CustomSet<TemplateParameterSubstitution, TemplateBinding> m_parameterSubstitutions = CustomSet<TemplateParameterSubstitution, TemplateBinding>(this);
            void restoreReference(Element* el) override;
            TypedSet<TemplateableElement, TemplateBinding>& getBoundElementSingleton();
            TypedSet<TemplateSignature, TemplateBinding>& getSignatureSingleton();
            void init();
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

#endif
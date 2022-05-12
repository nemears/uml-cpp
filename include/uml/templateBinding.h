#ifndef _UML_TEMPLATE_BINDING_H_
#define _UML_TEMPLATE_BINDING_H_

#include "directedRelationship.h"
#include "templateParameterSubstitution.h"
#include "singleton.h"

namespace UML {

    class TemplateParameterSubstitution;
    class TemplateableElement;
    class TemplateSignature;

    typedef UmlPtr<TemplateableElement> TemplateableElementPtr;
    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;

    class TemplateBinding : public DirectedRelationship {

        friend class UmlManager;
        friend class TemplateableElement;
        
        private:
            Singleton<TemplateableElement, TemplateBinding> m_boundElement = Singleton<TemplateableElement, TemplateBinding>(this);
            Singleton<TemplateSignature, TemplateBinding> m_signature = Singleton<TemplateSignature, TemplateBinding>(this);
            Set<TemplateParameterSubstitution, TemplateBinding> m_parameterSubstitutions = Set<TemplateParameterSubstitution, TemplateBinding>(this);
            void restoreReference(Element* el) override;
            Set<TemplateableElement, TemplateBinding>& getBoundElementSingleton();
            Set<TemplateSignature, TemplateBinding>& getSignatureSingleton();
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
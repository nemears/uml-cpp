#ifndef TEMPLATE_BINDING_H
#define TEMPLATE_BINDING_H

#include "directedRelationship.h"
#include "singleton.h"

namespace UML {

    class TemplateParameterSubstitution;
    class TemplateableElement;

    class TemplateBinding : public DirectedRelationship {

        friend class UmlManager;
        friend class TemplateableElement;
        
        private:
            Singleton<TemplateableElement, TemplateBinding> m_boundElement = Singleton<TemplateableElement, TemplateBinding>(this);
            Singleton<TemplateSignature, TemplateBinding> m_signature = Singleton<TemplateSignature, TemplateBinding>(this);
            Set<TemplateParameterSubstitution, TemplateBinding> m_parameterSubstitution = Set<TemplateParameterSubstitution, TemplateBinding>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReference(Element* el) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<TemplateableElement, TemplateBinding>& getBoundElementSingleton();
            Set<TemplateSignature, TemplateBinding>& getSignatureSingleton();
            void init();
            void copy(const TemplateBinding& rhs);
            TemplateBinding();
        public:
            TemplateBinding(const TemplateBinding& bind);
            ~TemplateBinding();
            TemplateableElement* getBoundElement(); 
            TemplateableElement& getBoundElementRef();
            ID getBoundElementID() const;
            bool hasBoundElement() const;
            void setBoundElement(TemplateableElement& el);
            void setBoundElement(TemplateableElement* el);
            void setBoundElement(ID id);
            TemplateSignature* getSignature();
            TemplateSignature& getSignatureRef();
            ID getSignatureID() const;
            bool hasSignature() const;
            void setSignature(TemplateSignature& signature);
            void setSignature(TemplateSignature* signature);
            void setSignature(ID id);
            Set<TemplateParameterSubstitution, TemplateBinding>& getParameterSubstitution();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_BINDING;
            };
    };
}

#endif
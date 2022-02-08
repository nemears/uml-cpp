#ifndef TEMPLATEABLE_ELEMENT_H
#define TEMPLATEABLE_ELEMENT_H

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateBinding;
    class TemplateSignature;

    class TemplateableElement : virtual public Element {

        friend class TemplateSignature;
        
        protected:
            Singleton<TemplateSignature, TemplateableElement> m_ownedTemplateSignature = Singleton<TemplateSignature, TemplateableElement>(this);
            Set<TemplateBinding, TemplateableElement> m_templateBindings = Set<TemplateBinding, TemplateableElement>(this);
            Set<TemplateSignature, TemplateableElement>& getOwnedTemplateSignatureSingleton();
            void init();
            void copy(const TemplateableElement& rhs);
            TemplateableElement();
        public:
            TemplateableElement(const TemplateableElement& el);
            virtual ~TemplateableElement();
            TemplateSignature* getOwnedTemplateSignature();
            TemplateSignature& getOwnedTemplateSignatureRef();
            ID getOwnedTemplateSignatureID() const;
            bool hasOwnedTemplateSignature() const;
            void setOwnedTemplateSignature(TemplateSignature& signature);
            void setOwnedTemplateSignature(TemplateSignature* signature);
            void setOwnedTemplateSignature(ID id);
            Set<TemplateBinding, TemplateableElement>& getTemplateBindings();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATEABLE_ELEMENT;
            };
    };
}

#endif
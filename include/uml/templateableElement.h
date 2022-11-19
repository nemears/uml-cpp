#ifndef _UML_TEMPLATEABLE_ELEMENT_H_
#define _UML_TEMPLATEABLE_ELEMENT_H_

#include "element.h"
#include "set/singleton.h"
#include "templateBinding.h"
#include "templateSignature.h"

namespace UML {

    class TemplateSignature;

    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;

    class TemplateableElement : virtual public Element {

        friend class TemplateSignature;
        
        protected:
            CustomSingleton<TemplateSignature, TemplateableElement> m_ownedTemplateSignature = CustomSingleton<TemplateSignature, TemplateableElement>(this);
            CustomSet<TemplateBinding, TemplateableElement> m_templateBindings = CustomSet<TemplateBinding, TemplateableElement>(this);
            TypedSet<TemplateSignature, TemplateableElement>& getOwnedTemplateSignatureSingleton();
            void init();
            TemplateableElement();
        public:
            virtual ~TemplateableElement();
            TemplateSignaturePtr getOwnedTemplateSignature() const;
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
#ifndef _UML_TEMPLATEABLE_ELEMENT_H_
#define _UML_TEMPLATEABLE_ELEMENT_H_

#include "element.h"
#include "uml/set/singleton.h"
#include "templateBinding.h"
#include "templateSignature.h"

namespace UML {

    class TemplateSignature;

    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;

    class TemplateableElement : virtual public Element {

        friend class TemplateSignature;
        
        protected:
            Singleton<TemplateSignature, TemplateableElement> m_ownedTemplateSignature = Singleton<TemplateSignature, TemplateableElement>(this);
            Set<TemplateBinding, TemplateableElement> m_templateBindings = Set<TemplateBinding, TemplateableElement>(this);
            Singleton<TemplateSignature, TemplateableElement>& getOwnedTemplateSignatureSingleton();
            TemplateableElement();
        public:
            virtual ~TemplateableElement();
            TemplateSignaturePtr getOwnedTemplateSignature() const;
            void setOwnedTemplateSignature(TemplateSignature& signature);
            void setOwnedTemplateSignature(TemplateSignaturePtr signature);
            void setOwnedTemplateSignature(ID id);
            Set<TemplateBinding, TemplateableElement>& getTemplateBindings();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATEABLE_ELEMENT;
            };
    };
}

#endif

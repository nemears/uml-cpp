#pragma once

#include "element.h"
#include "uml/set/singleton.h"
#include "templateBinding.h"
#include "templateSignature.h"

namespace UML {

    class TemplateSignature;

    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;

    class TemplateableElement : virtual public Element {

        friend class TemplateSignature;
        friend struct ElementInfo<TemplateableElement>;
        
        protected:
            Singleton<TemplateSignature, TemplateableElement> m_ownedTemplateSignature = Singleton<TemplateSignature, TemplateableElement>(this);
            Set<TemplateBinding, TemplateableElement> m_templateBindings = Set<TemplateBinding, TemplateableElement>(this);
            Singleton<TemplateSignature, TemplateableElement>& getOwnedTemplateSignatureSingleton();
            TemplateableElement(std::size_t elementType, AbstractManager& manager);
        public:
            TemplateSignaturePtr getOwnedTemplateSignature() const;
            void setOwnedTemplateSignature(TemplateSignature& signature);
            void setOwnedTemplateSignature(TemplateSignaturePtr signature);
            void setOwnedTemplateSignature(ID id);
            Set<TemplateBinding, TemplateableElement>& getTemplateBindings();
            typedef TypeInfo<std::tuple<Element>, TemplateableElement> Info;
    };

    template <>
    struct ElementInfo<TemplateableElement> : public DefaultInfo {
        static const bool abstract = true;
        static constexpr std::string_view name {"TemplateableElement"};
        static SetList sets(TemplateableElement& el) {
            return SetList {
                makeSetPair("ownedTemplateSignature", el.m_ownedTemplateSignature),
                makeSetPair("templateBindings", el.m_templateBindings)
            };
        }
    };
}


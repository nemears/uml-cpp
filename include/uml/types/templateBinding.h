#pragma once

#include "directedRelationship.h"
#include "uml/set/singleton.h"
#include "uml/types/element.h"

namespace UML {

    class TemplateParameterSubstitution;
    class TemplateableElement;
    class TemplateSignature;

    typedef UmlPtr<TemplateableElement> TemplateableElementPtr;
    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;

    class TemplateBinding : public DirectedRelationship {

        friend class TemplateableElement;
        friend struct ElementInfo<TemplateBinding>;
        
        protected:
            Singleton<TemplateableElement, TemplateBinding> m_boundElement = Singleton<TemplateableElement, TemplateBinding>(this);
            Singleton<TemplateSignature, TemplateBinding> m_signature = Singleton<TemplateSignature, TemplateBinding>(this);
            Set<TemplateParameterSubstitution, TemplateBinding> m_parameterSubstitutions = Set<TemplateParameterSubstitution, TemplateBinding>(this);
            Singleton<TemplateableElement, TemplateBinding>& getBoundElementSingleton();
            Singleton<TemplateSignature, TemplateBinding>& getSignatureSingleton();
            TemplateBinding(std::size_t elementType, AbstractManager& manager);
        public:
            TemplateableElementPtr getBoundElement() const;
            void setBoundElement(TemplateableElement& el);
            void setBoundElement(TemplateableElementPtr el);
            void setBoundElement(ID id);
            TemplateSignaturePtr getSignature() const;
            void setSignature(TemplateSignature& signature);
            void setSignature(TemplateSignaturePtr signature);
            void setSignature(ID id);
            Set<TemplateParameterSubstitution, TemplateBinding>& getParameterSubstitutions();
            typedef TypeInfo<std::tuple<DirectedRelationship>, TemplateBinding> Info;
    };

    template <>
    struct ElementInfo<TemplateBinding> {
        static const bool abstract = false;
        inline static const std::string name {"TemplateBinding"};
        static SetList sets(TemplateBinding& el) {
            return SetList {
                makeSetPair("boundElement", el.m_boundElement),
                makeSetPair("signature", el.m_signature),
                makeSetPair("templateParameterSubstitutions", el.m_parameterSubstitutions)
            };
        }
    };
}

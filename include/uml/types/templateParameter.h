#pragma once

#include "element.h"
#include "uml/set/singleton.h"

namespace UML {

    class TemplateSignature;
    class ParameterableElement;
    class TemplateParameterSubstitution;

    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;
    typedef UmlPtr<ParameterableElement> ParameterableElementPtr;

    class TemplateParameter : virtual public Element {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class TemplateParameterSubstitution;
        friend class TemplateSignature;
        friend class ParameterableElement;
        
        protected:
            Singleton<TemplateSignature, TemplateParameter> m_signature = Singleton<TemplateSignature, TemplateParameter>(this);
            Singleton<ParameterableElement, TemplateParameter> m_parameteredElement = Singleton<ParameterableElement, TemplateParameter>(this);
            Singleton<ParameterableElement, TemplateParameter> m_ownedParameteredElement = Singleton<ParameterableElement, TemplateParameter>(this);
            Singleton<ParameterableElement, TemplateParameter> m_default = Singleton<ParameterableElement, TemplateParameter>(this);
            Singleton<ParameterableElement, TemplateParameter> m_ownedDefault = Singleton<ParameterableElement, TemplateParameter>(this);
            void referenceErased(ID id) override;
            Singleton<TemplateSignature, TemplateParameter>& getSignatureSingleton();
            Singleton<ParameterableElement, TemplateParameter>& getParameteredElementSingleton();
            Singleton<ParameterableElement, TemplateParameter>& getOwnedParameteredElementSingleton();
            Singleton<ParameterableElement, TemplateParameter>& getDefaultSingleton();
            Singleton<ParameterableElement, TemplateParameter>& getOwnedDefaultSingleton();
            TemplateParameter();
        public:
            virtual ~TemplateParameter();
            TemplateSignaturePtr getSignature() const;
            void setSignature(TemplateSignature& signature);
            void setSignature(TemplateSignaturePtr signature);
            void setSignature(ID id);
            ParameterableElementPtr getOwnedParameteredElement() const;
            void setOwnedParameteredElement(ParameterableElement& el);
            void setOwnedParameteredElement(ParameterableElementPtr el);
            void setOwnedParameteredElement(ID id);
            ParameterableElementPtr getParameteredElement() const;
            void setParameteredElement(ParameterableElement& el);
            void setParameteredElement(ParameterableElementPtr el);
            void setParameteredElement(ID id);
            ParameterableElementPtr getDefault() const;
            void setDefault(ParameterableElement& el);
            void setDefault(ParameterableElementPtr el);
            void setDefault(ID id);
            ParameterableElementPtr getOwnedDefault() const;
            void setOwnedDefault(ParameterableElement& el);
            void setOwnedDefault(ParameterableElementPtr el);
            void setOwnedDefault(ID id);
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER;
            };
    };
}

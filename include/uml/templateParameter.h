#ifndef _UML_TEMPLATE_PARAMETER_H_
#define _UML_TEMPLATE_PARAMETER_H_

#include "element.h"
#include "set/singleton.h"

namespace UML {

    class TemplateSignature;
    class ParameterableElement;
    class TemplateParameterSubstitution;

    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;
    typedef UmlPtr<ParameterableElement> ParameterableElementPtr;

    class TemplateParameter : virtual public Element {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class TemplateParameterSubstitution;
        friend class TemplateSignature;
        friend class ParameterableElement;
        
        protected:
            CustomSingleton<TemplateSignature, TemplateParameter> m_signature = CustomSingleton<TemplateSignature, TemplateParameter>(this);
            CustomSingleton<ParameterableElement, TemplateParameter> m_parameteredElement = CustomSingleton<ParameterableElement, TemplateParameter>(this);
            CustomSingleton<ParameterableElement, TemplateParameter> m_ownedParameteredElement = CustomSingleton<ParameterableElement, TemplateParameter>(this);
            CustomSingleton<ParameterableElement, TemplateParameter> m_default = CustomSingleton<ParameterableElement, TemplateParameter>(this);
            CustomSingleton<ParameterableElement, TemplateParameter> m_ownedDefault = CustomSingleton<ParameterableElement, TemplateParameter>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            TypedSet<TemplateSignature, TemplateParameter>& getSignatureSingleton();
            TypedSet<ParameterableElement, TemplateParameter>& getParameteredElementSingleton();
            TypedSet<ParameterableElement, TemplateParameter>& getOwnedParameteredElementSingleton();
            TypedSet<ParameterableElement, TemplateParameter>& getDefaultSingleton();
            TypedSet<ParameterableElement, TemplateParameter>& getOwnedDefaultSingleton();
            void init();
            TemplateParameter();
        public:
            virtual ~TemplateParameter();
            TemplateSignaturePtr getSignature() const;
            void setSignature(TemplateSignature& signature);
            void setSignature(TemplateSignature* signature);
            void setSignature(ID id);
            ParameterableElementPtr getOwnedParameteredElement() const;
            void setOwnedParameteredElement(ParameterableElement& el);
            void setOwnedParameteredElement(ParameterableElement* el);
            void setOwnedParameteredElement(ID id);
            ParameterableElementPtr getParameteredElement() const;
            void setParameteredElement(ParameterableElement& el);
            void setParameteredElement(ParameterableElement* el);
            void setParameteredElement(ID id);
            ParameterableElementPtr getDefault() const;
            void setDefault(ParameterableElement& el);
            void setDefault(ParameterableElement* el);
            void setDefault(ID id);
            ParameterableElementPtr getOwnedDefault() const;
            void setOwnedDefault(ParameterableElement& el);
            void setOwnedDefault(ParameterableElement* el);
            void setOwnedDefault(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER;
            };
    };
}

#endif
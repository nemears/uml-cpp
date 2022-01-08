#ifndef TEMPLATE_PARAMETER_H
#define TEMPLATE_PARAMETER_H

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateSignature;
    class ParameterableElement;
    class TemplateParameterSubstitution;

    class TemplateParameter : public Element {

        friend class UmlManager;
        friend class TemplateParameterSubstitution;
        friend class TemplateSignature;
        friend class ParameterableElement;
        
        private:
            Singleton<TemplateSignature, TemplateParameter> m_signature = Singleton<TemplateSignature, TemplateParameter>(this);
            Singleton<ParameterableElement, TemplateParameter> m_parameteredElement = Singleton<ParameterableElement, TemplateParameter>(this);
            Singleton<ParameterableElement, TemplateParameter> m_ownedParameteredElement = Singleton<ParameterableElement, TemplateParameter>(this);
            Singleton<ParameterableElement, TemplateParameter> m_default = Singleton<ParameterableElement, TemplateParameter>(this);
            Singleton<ParameterableElement, TemplateParameter> m_ownedDefault = Singleton<ParameterableElement, TemplateParameter>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<TemplateSignature, TemplateParameter>& getSignatureSingleton();
            Set<ParameterableElement, TemplateParameter>& getParameteredElementSingleton();
            Set<ParameterableElement, TemplateParameter>& getOwnedParameteredElementSingleton();
            Set<ParameterableElement, TemplateParameter>& getDefaultSingleton();
            Set<ParameterableElement, TemplateParameter>& getOwnedDefaultSingleton();
            void init();
            void copy(const TemplateParameter& rhs);
            TemplateParameter();
        public:
            TemplateParameter(const TemplateParameter& el);
            virtual ~TemplateParameter();
            TemplateSignature* getSignature();
            TemplateSignature& getSignatureRef();
            ID getSignatureID() const;
            bool hasSignature() const;
            void setSignature(TemplateSignature& signature);
            void setSignature(TemplateSignature* signature);
            void setSignature(ID id);
            ParameterableElement* getOwnedParameteredElement();
            ParameterableElement& getOwnedParameteredElementRef();
            ID getOwnedParameteredElementID() const;
            bool hasOwnedParameteredElement() const;
            void setOwnedParameteredElement(ParameterableElement& el);
            void setOwnedParameteredElement(ParameterableElement* el);
            void setOwnedParameteredElement(ID id);
            ParameterableElement* getParameteredElement();
            ParameterableElement& getParameteredElementRef();
            ID getParameteredElementID() const;
            bool hasParameteredElement() const;
            void setParameteredElement(ParameterableElement& el);
            void setParameteredElement(ParameterableElement* el);
            void setParameteredElement(ID id);
            ParameterableElement* getDefault();
            ParameterableElement& getDefaultRef();
            ID getDefaultID() const;
            bool hasDefault() const;
            void setDefault(ParameterableElement& el);
            void setDefault(ParameterableElement* el);
            void setDefault(ID id);
            ParameterableElement* getOwnedDefault();
            ParameterableElement& getOwnedDefaultRef();
            ID getOwnedDefaultID() const;
            bool hasOwnedDefault() const;
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
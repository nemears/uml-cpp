#ifndef _UML_TEMPLATE_PARAMETER_H_
#define _UML_TEMPLATE_PARAMETER_H_

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateSignature;
    class ParameterableElement;
    class TemplateParameterSubstitution;

    typedef UmlPtr<TemplateSignature> TemplateSignaturePtr;
    typedef UmlPtr<ParameterableElement> ParameterableElementPtr;

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
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            Set<TemplateSignature, TemplateParameter>& getSignatureSingleton();
            Set<ParameterableElement, TemplateParameter>& getParameteredElementSingleton();
            Set<ParameterableElement, TemplateParameter>& getOwnedParameteredElementSingleton();
            Set<ParameterableElement, TemplateParameter>& getDefaultSingleton();
            Set<ParameterableElement, TemplateParameter>& getOwnedDefaultSingleton();
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
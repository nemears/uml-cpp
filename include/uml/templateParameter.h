#ifndef TEMPLATE_PARAMETER_H
#define TEMPLATE_PARAMETER_H

#include "element.h"
#include "singleton.h"

namespace UML {

    class TemplateSignature;
    class ParameterableElement;
    class TemplateParameterSubstitution;
    namespace Parsers {
        class SetSignature;
        class SetOwnedParameteredElement;
        class SetOwnedDefault;
    }

    class TemplateParameter : public Element {

        friend class UmlManager;
        friend class TemplateParameterSubstitution;
        friend class Parsers::SetSignature;
        friend class Parsers::SetOwnedParameteredElement;
        friend class Parsers::SetOwnedDefault;

        private:
            Singleton<TemplateSignature, TemplateParameter> m_signature = Singleton<TemplateSignature, TemplateParameter>(this);
            bool m_setFlag = false;
            class RemoveSignatureProcedure : public AbstractSingletonProcedure<TemplateSignature, TemplateParameter> {
                public:
                    RemoveSignatureProcedure(TemplateParameter* me) : AbstractSingletonProcedure<TemplateSignature, TemplateParameter>(me) {};
                    void operator()(TemplateSignature* el) const override;
            };
            class AddSignatureProcedure : public AbstractSingletonProcedure<TemplateSignature, TemplateParameter> {
                public:
                    AddSignatureProcedure(TemplateParameter* me) : AbstractSingletonProcedure<TemplateSignature, TemplateParameter>(me) {};
                    void operator()(TemplateSignature* el) const override;
                    void operator()(ID id) const override;
            };
            Singleton<ParameterableElement, TemplateParameter> m_ownedParameteredElement = Singleton<ParameterableElement, TemplateParameter>(this);
            class RemoveOwnedParameteredElementProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameter> {
                public:
                    RemoveOwnedParameteredElementProcedure(TemplateParameter* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameter>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            class AddOwnedParameteredElementProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameter> {
                public:
                    AddOwnedParameteredElementProcedure(TemplateParameter* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameter>(me) {};
                    void operator()(ParameterableElement* el) const override;
                    void operator()(ID id) const override;
            };
            Singleton<ParameterableElement, TemplateParameter> m_parameteredElement = Singleton<ParameterableElement, TemplateParameter>(this);
            class RemoveParameteredElementProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameter> {
                public:
                    RemoveParameteredElementProcedure(TemplateParameter* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameter>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            class AddParameteredElementProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameter> {
                public:
                    AddParameteredElementProcedure(TemplateParameter* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameter>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            Singleton<ParameterableElement, TemplateParameter> m_default = Singleton<ParameterableElement, TemplateParameter>(this);
            class RemoveDefaultProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameter> {
                public:
                    RemoveDefaultProcedure(TemplateParameter* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameter>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            class AddDefaultProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameter> {
                public:
                    AddDefaultProcedure(TemplateParameter* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameter>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            Singleton<ParameterableElement, TemplateParameter> m_ownedDefault = Singleton<ParameterableElement, TemplateParameter>(this);
            class RemoveOwnedDefaultProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameter> {
                public:
                    RemoveOwnedDefaultProcedure(TemplateParameter* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameter>(me) {};
                    void operator()(ParameterableElement* el) const override;
            };
            class AddOwnedDefaultProcedure : public AbstractSingletonProcedure<ParameterableElement, TemplateParameter> {
                public:
                    AddOwnedDefaultProcedure(TemplateParameter* me) : AbstractSingletonProcedure<ParameterableElement, TemplateParameter>(me) {};
                    void operator()(ParameterableElement* el) const override;
                    void operator()(ID id) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
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
            ParameterableElement* getOwnedParameteredElement();
            ParameterableElement& getOwnedParameteredElementRef();
            ID getOwnedParameteredElementID() const;
            bool hasOwnedParameteredElement() const;
            void setOwnedParameteredElement(ParameterableElement& el);
            void setOwnedParameteredElement(ParameterableElement* el);
            ParameterableElement* getParameteredElement();
            ParameterableElement& getParameteredElementRef();
            ID getParameteredElementID() const;
            bool hasParameteredElement() const;
            void setParameteredElement(ParameterableElement& el);
            void setParameteredElement(ParameterableElement* el);
            ParameterableElement* getDefault();
            ParameterableElement& getDefaultRef();
            ID getDefaultID() const;
            bool hasDefault() const;
            void setDefault(ParameterableElement& el);
            void setDefault(ParameterableElement* el);
            ParameterableElement* getOwnedDefault();
            ParameterableElement& getOwnedDefaultRef();
            ID getOwnedDefaultID() const;
            bool hasOwnedDefault() const;
            void setOwnedDefault(ParameterableElement& el);
            void setOwnedDefault(ParameterableElement* el);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_PARAMETER;
            };
    };
}

#endif
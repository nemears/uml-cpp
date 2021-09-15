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
            bool m_setFlag = false;
            class RemoveOwnedTemplateSignatureProcedure : public AbstractSingletonProcedure<TemplateSignature, TemplateableElement> {
                private:
                public:
                    RemoveOwnedTemplateSignatureProcedure(TemplateableElement* me) : AbstractSingletonProcedure<TemplateSignature, TemplateableElement>(me) {};
                    void operator()(TemplateSignature* el) const override;
            };
            class AddOwnedTemplateSignatureProcedure : public AbstractSingletonProcedure<TemplateSignature, TemplateableElement> {
                public:
                    AddOwnedTemplateSignatureProcedure(TemplateableElement* me) : AbstractSingletonProcedure<TemplateSignature, TemplateableElement>(me) {};
                    void operator()(TemplateSignature* el) const override;
            };
            Singleton<TemplateBinding, TemplateableElement> m_templateBinding = Singleton<TemplateBinding, TemplateableElement>(this);
            class RemoveTemplateBindingProcedure : public AbstractSingletonProcedure<TemplateBinding, TemplateableElement> {
                public:
                    RemoveTemplateBindingProcedure(TemplateableElement* me) : AbstractSingletonProcedure<TemplateBinding, TemplateableElement>(me) {};
                    void operator()(TemplateBinding* el) const override;
            };
            class AddTemplateBindingProcedure : public AbstractSingletonProcedure<TemplateBinding, TemplateableElement> {
                public:
                    AddTemplateBindingProcedure(TemplateableElement* me) : AbstractSingletonProcedure<TemplateBinding, TemplateableElement>(me) {};
                    void operator()(TemplateBinding* el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            TemplateableElement();
        public:
            TemplateableElement(const TemplateableElement& el);
            virtual ~TemplateableElement();
            TemplateSignature* getOwnedTemplateSignature();
            TemplateSignature& getOwnedTemplateSignatureRef();
            bool hasOwnedTemplateSignature() const;
            void setOwnedTemplateSignature(TemplateSignature& signature);
            void setOwnedTemplateSignature(TemplateSignature* signature);
            TemplateBinding* getTemplateBinding();
            TemplateBinding& getTemplateBindingRef();
            bool hasTemplateBinding() const;
            void setTemplateBinding(TemplateBinding& binding);
            void setTemplateBinding(TemplateBinding* binding);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATEABLE_ELEMENT;
            };
    };
}

#endif
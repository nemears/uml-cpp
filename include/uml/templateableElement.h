#ifndef TEMPLATEABLE_ELEMENT_H
#define TEMPLATEABLE_ELEMENT_H

#include "element.h"
#include "singleton.h"
#include "sequence.h"

namespace UML {

    class TemplateBinding;
    class TemplateSignature;
    namespace Parsers {
        class SetOwnedTemplateSignature;
    }

    class TemplateableElement : virtual public Element {

        friend class TemplateSignature;
        friend class Parsers::SetOwnedTemplateSignature;

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
                    void operator()(ID id) const override;
            };
            Sequence<TemplateBinding> m_templateBindings = Sequence<TemplateBinding>(this);
            class AddTemplateBindingFunctor : public TemplateAbstractSequenceFunctor<TemplateBinding, TemplateableElement> {
                public:
                    AddTemplateBindingFunctor(TemplateableElement* el) : TemplateAbstractSequenceFunctor(el) {};
                    void operator()(TemplateBinding& el) const override;
            };
            class RemoveTemplateBindingFunctor : public TemplateAbstractSequenceFunctor<TemplateBinding, TemplateableElement> {
                public:
                    RemoveTemplateBindingFunctor(TemplateableElement* el) : TemplateAbstractSequenceFunctor(el) {};
                    void operator()(TemplateBinding& el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            TemplateableElement();
        public:
            TemplateableElement(const TemplateableElement& el);
            virtual ~TemplateableElement();
            TemplateSignature* getOwnedTemplateSignature();
            TemplateSignature& getOwnedTemplateSignatureRef();
            ID getOwnedTemplateSignatureID() const;
            bool hasOwnedTemplateSignature() const;
            void setOwnedTemplateSignature(TemplateSignature& signature);
            void setOwnedTemplateSignature(TemplateSignature* signature);
            Sequence<TemplateBinding>& getTemplateBindings();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATEABLE_ELEMENT;
            };
    };
}

#endif
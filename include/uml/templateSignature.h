#ifndef _UML_TEMPLATE_SIGNATURE_H_
#define _UML_TEMPLATE_SIGNATURE_H_

#include "element.h"
#include "templateParameter.h"
#include "set/singleton.h"
#include "set/orderedSet.h"

namespace UML {

    class TemplateableElement;
    class TemplateParameter;

    typedef UmlPtr<TemplateableElement> TemplateableElementPtr;

    class TemplateSignature : virtual public Element {

        template <typename AccessPolicy, typename SerializationPolicy> friend class Manager;
        friend class TemplateParameter;
        friend class TemplateableElement;

        protected:
            CustomSingleton<TemplateableElement, TemplateSignature> m_template = CustomSingleton<TemplateableElement, TemplateSignature>(this);
            CustomOrderedSet<TemplateParameter, TemplateSignature> m_parameters = CustomOrderedSet<TemplateParameter, TemplateSignature>(this);
            CustomOrderedSet<TemplateParameter, TemplateSignature> m_ownedParameters = CustomOrderedSet<TemplateParameter, TemplateSignature>(this);
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
            TypedSet<TemplateableElement, TemplateSignature>& getTemplateSingleton();
            TemplateSignature();
        public:
            virtual ~TemplateSignature();
            TemplateableElementPtr getTemplate() const;
            void setTemplate(TemplateableElement& temp);
            void setTemplate(TemplateableElement* temp);
            void setTemplate(ID id);
            OrderedSet<TemplateParameter, TemplateSignature>& getOwnedParameters();
            OrderedSet<TemplateParameter, TemplateSignature>& getParameters();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_SIGNATURE;
            };
    };
}

#endif
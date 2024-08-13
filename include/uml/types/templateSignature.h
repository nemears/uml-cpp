#pragma once

#include "element.h"
#include "uml/set/singleton.h"
#include "uml/set/orderedSet.h"

namespace UML {

    class TemplateableElement;
    class TemplateParameter;

    typedef UmlPtr<TemplateableElement> TemplateableElementPtr;

    class TemplateSignature : virtual public Element {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class TemplateParameter;
        friend class TemplateableElement;

        protected:
            Singleton<TemplateableElement, TemplateSignature> m_template = Singleton<TemplateableElement, TemplateSignature>(this);
            OrderedSet<TemplateParameter, TemplateSignature> m_parameters = OrderedSet<TemplateParameter, TemplateSignature>(this);
            OrderedSet<TemplateParameter, TemplateSignature> m_ownedParameters = OrderedSet<TemplateParameter, TemplateSignature>(this);
            void referenceErased(ID id) override;
            Singleton<TemplateableElement, TemplateSignature>& getTemplateSingleton();
            TemplateSignature();
        public:
            virtual ~TemplateSignature();
            TemplateableElementPtr getTemplate() const;
            void setTemplate(TemplateableElement& temp);
            void setTemplate(TemplateableElementPtr temp);
            void setTemplate(ID id);
            OrderedSet<TemplateParameter, TemplateSignature>& getOwnedParameters();
            OrderedSet<TemplateParameter, TemplateSignature>& getParameters();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_SIGNATURE;
            };
    };
}

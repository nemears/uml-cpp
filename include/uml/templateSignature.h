#ifndef TEMPLATE_SIGNATURE_H
#define TEMPLATE_SIGNATURE_H

#include "element.h"
#include "singleton.h"
#include "orderedSet.h"

namespace UML {

    class TemplateableElement;
    class TemplateParameter;
    namespace Parsers {
        class SetTemplate;
    }

    class TemplateSignature : public Element {

        friend class UmlManager;
        friend class TemplateParameter;
        friend class Parsers::SetTemplate;
        friend class TemplateableElement;

        private:
            Singleton<TemplateableElement, TemplateSignature> m_template = Singleton<TemplateableElement, TemplateSignature>(this);
            OrderedSet<TemplateParameter, TemplateSignature> m_parameters = OrderedSet<TemplateParameter, TemplateSignature>(this);
            OrderedSet<TemplateParameter, TemplateSignature> m_ownedParameters =  OrderedSet<TemplateParameter, TemplateSignature>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<TemplateableElement, TemplateSignature>& getTemplateSingleton();
            Set<TemplateParameter, TemplateSignature>& getParametersSet();
            Set<TemplateParameter, TemplateSignature>& getOwnedParametersSet();
            void init();
            void copy(const TemplateSignature& rhs);
            TemplateSignature();
        public:
            TemplateSignature(const TemplateSignature& el);
            virtual ~TemplateSignature();
            TemplateableElement* getTemplate();
            TemplateableElement& getTemplateRef();
            ID getTemplateID() const;
            bool hasTemplate() const;
            void setTemplate(TemplateableElement& temp);
            void setTemplate(TemplateableElement* temp);
            OrderedSet<TemplateParameter, TemplateSignature>& getOwnedParameters();
            OrderedSet<TemplateParameter, TemplateSignature>& getParameters();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_SIGNATURE;
            };
    };
}

#endif
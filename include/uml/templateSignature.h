#ifndef TEMPLATE_SIGNATURE_H
#define TEMPLATE_SIGNATURE_H

#include "element.h"
#include "sequence.h"

namespace UML {

    class TemplateableElement;
    class TemplateParameter;
    namespace Parsers {
        class SetTemplate;
    }

    class TemplateSignature : public Element {

        friend class UmlManager;
        friend class Parsers::SetTemplate;
        friend class TemplateableElement;

        private:
            Singleton<TemplateableElement, TemplateSignature> m_template = Singleton<TemplateableElement, TemplateSignature>(this);
            // Sequence<TemplateParameter> m_ownedParameter =  Sequence<TemplateParameter>(this);
            // Sequence<TemplateParameter> m_parameter = Sequence<TemplateParameter>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<TemplateableElement, TemplateSignature>& getTemplateSingleton();
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
            // Sequence<TemplateParameter>& getOwnedParameter();
            // Sequence<TemplateParameter>& getParameter();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATE_SIGNATURE;
            };
    };
}

#endif
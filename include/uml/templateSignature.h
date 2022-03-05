#ifndef _UML_TEMPLATE_SIGNATURE_H_
#define _UML_TEMPLATE_SIGNATURE_H_

#include "element.h"
#include "templateParameter.h"
#include "singleton.h"
#include "orderedSet.h"

namespace UML {

    class TemplateableElement;
    class TemplateParameter;

    typedef UmlPtr<TemplateableElement> TemplateableElementPtr;

    class TemplateSignature : public Element {

        friend class UmlManager;
        friend class TemplateParameter;
        friend class TemplateableElement;

        private:
            Singleton<TemplateableElement, TemplateSignature> m_template = Singleton<TemplateableElement, TemplateSignature>(this);
            OrderedSet<TemplateParameter, TemplateSignature> m_parameters = OrderedSet<TemplateParameter, TemplateSignature>(this);
            OrderedSet<TemplateParameter, TemplateSignature> m_ownedParameters =  OrderedSet<TemplateParameter, TemplateSignature>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referenceErased(ID id) override;
            Set<TemplateableElement, TemplateSignature>& getTemplateSingleton();
            Set<TemplateParameter, TemplateSignature>& getParametersSet();
            Set<TemplateParameter, TemplateSignature>& getOwnedParametersSet();
            void init();
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
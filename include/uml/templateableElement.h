#ifndef TEMPLATEABLE_ELEMENT_H
#define TEMPLATEABLE_ELEMENT_H

#include "element.h"
#include "singleton.h"
#include "yaml-cpp/yaml.h"

namespace UML {

    class TemplateBinding;
    class TemplateSignature;
    namespace Parsers {
        void parseTemplateableElement(YAML::Node node, TemplateableElement& el, Parsers::ParserMetaData& data);
    }

    class TemplateableElement : virtual public Element {

        friend class TemplateSignature;
        friend void Parsers::parseTemplateableElement(YAML::Node node, TemplateableElement& el, Parsers::ParserMetaData& data);

        protected:
            Singleton<TemplateSignature, TemplateableElement> m_ownedTemplateSignature = Singleton<TemplateSignature, TemplateableElement>(this);
            Set<TemplateBinding, TemplateableElement> m_templateBindings = Set<TemplateBinding, TemplateableElement>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Set<TemplateSignature, TemplateableElement>& getOwnedTemplateSignatureSingleton();
            void init();
            void copy(const TemplateableElement& rhs);
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
            Set<TemplateBinding, TemplateableElement>& getTemplateBindings();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TEMPLATEABLE_ELEMENT;
            };
    };
}

#endif
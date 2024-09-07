#pragma once

#include "element.h"
#include "uml/set/singleton.h"
#include "uml/set/orderedSet.h"

namespace UML {

    class TemplateableElement;
    class TemplateParameter;

    typedef UmlPtr<TemplateableElement> TemplateableElementPtr;

    class TemplateSignature : virtual public Element {

        friend class TemplateParameter;
        friend class TemplateableElement;
        friend struct ElementInfo<TemplateSignature>;

        protected:
            Singleton<TemplateableElement, TemplateSignature> m_template = Singleton<TemplateableElement, TemplateSignature>(this);
            OrderedSet<TemplateParameter, TemplateSignature> m_parameters = OrderedSet<TemplateParameter, TemplateSignature>(this);
            OrderedSet<TemplateParameter, TemplateSignature> m_ownedParameters = OrderedSet<TemplateParameter, TemplateSignature>(this);
            Singleton<TemplateableElement, TemplateSignature>& getTemplateSingleton();
        public:
            TemplateSignature(std::size_t elementType, AbstractManager& manager);
            TemplateableElementPtr getTemplate() const;
            void setTemplate(TemplateableElement& temp);
            void setTemplate(TemplateableElementPtr temp);
            void setTemplate(ID id);
            OrderedSet<TemplateParameter, TemplateSignature>& getOwnedParameters();
            OrderedSet<TemplateParameter, TemplateSignature>& getParameters();
            typedef TypeInfo<std::tuple<Element>, TemplateSignature> Info;
    };

    template <>
    struct ElementInfo<TemplateSignature> {
        static const bool abstract = false;
        inline static const std::string name {"TemplateSignature"};
        static SetList sets(TemplateSignature& el) {
            return SetList {
                makeSetPair("template", el.m_template),
                makeSetPair("parameters", el.getParameters()),
                makeSetPair("ownedParameters", el.getParameters())
            };
        }
    };
}

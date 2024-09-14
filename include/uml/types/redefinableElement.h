#pragma once

#include "namedElement.h"
#include "uml/set/set.h"
#include "uml/types/element.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        protected:
            ReadOnlySet<RedefinableElement, RedefinableElement> m_redefinedElement = ReadOnlySet<RedefinableElement, RedefinableElement>(this);
            ReadOnlySet<Classifier, RedefinableElement> m_redefinitionContext = ReadOnlySet<Classifier, RedefinableElement>(this);
            RedefinableElement(std::size_t elementType, AbstractManager& manager);
        public:
            ReadOnlySet<RedefinableElement, RedefinableElement>& getRedefinedElements();
            ReadOnlySet<Classifier, RedefinableElement>& getRedefinitionContext();
            typedef TypeInfo<std::tuple<NamedElement>, RedefinableElement> Info;
    };

    template <>
    struct ElementInfo<RedefinableElement> : public DefaultInfo {
        static const bool abstract = true;
        inline static const std::string name {"RedefinableElement"};
        static SetList sets(RedefinableElement& el) {
            return SetList {
                makeSetPair("redefinedElements", el.getRedefinedElements()),
                makeSetPair("redefinitionContext", el.getRedefinitionContext())
            };
        }
    };
}

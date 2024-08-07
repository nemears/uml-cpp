#pragma once

#include "uml/set/set.h"

namespace UML {
    class Relationship : virtual public Element {
        protected:
            ReadOnlySet<Element, Relationship> m_relatedElements = ReadOnlySet<Element, Relationship>(this);
            void referenceErased(ID id) override;
            Relationship();
        public:
            virtual ~Relationship();
            ReadOnlySet<Element, Relationship>& getRelatedElements();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::RELATIONSHIP;
            };
    };
}

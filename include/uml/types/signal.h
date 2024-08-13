#pragma once

#include "classifier.h"
#include "uml/set/orderedSet.h"

namespace UML {
    class Signal : public Classifier {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            IndexableOrderedSet<Property, Signal> m_ownedAttributes = IndexableOrderedSet<Property, Signal>(this);
            Signal();
        public:
            virtual ~Signal();
            IndexableOrderedSet<Property, Signal>& getOwnedAttributes();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::SIGNAL;
            };
    };
}

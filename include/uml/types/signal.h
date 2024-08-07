#pragma once

#include "classifier.h"
#include "uml/set/orderedSet.h"

namespace UML {
    class Signal : public Classifier {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            OrderedSet<Property, Signal> m_ownedAttributes = OrderedSet<Property, Signal>(this);
            Signal();
        public:
            virtual ~Signal();
            OrderedSet<Property, Signal>& getOwnedAttributes();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::SIGNAL;
            };
    };
}

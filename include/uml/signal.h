#ifndef _UML_SIGNAL_H_
#define _UML_SIGNAL_H_

#include "classifier.h"
#include "set/orderedSet.h"

namespace UML {
    class Signal : public Classifier {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomOrderedSet<Property, Signal> m_ownedAttributes = CustomOrderedSet<Property, Signal>(this);
            // Set<Property, Signal>& getOwnedAttributesSet();
            void init();
            Signal();
        public:
            virtual ~Signal();
            OrderedSet<Property, Signal>& getOwnedAttributes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::SIGNAL;
            };
    };
}

#endif
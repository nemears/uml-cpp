#ifndef _UML_SIGNAL_H_
#define _UML_SIGNAL_H_

#include "classifier.h"
#include "orderedSet.h"

namespace UML {
    class Signal : public Classifier {

        friend class UmlManager;

        protected:
            OrderedSet<Property, Signal> m_ownedAttributes = OrderedSet<Property, Signal>(this);
            Set<Property, Signal>& getOwnedAttributesSet();
            void init();
            void copy(const Signal& rhs);
            Signal();
        public:
            Signal(const Signal& rhs);
            virtual ~Signal();
            OrderedSet<Property, Signal>& getOwnedAttributes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::SIGNAL;
            };
    };
}

#endif
#ifndef _UML_INTERFACE_H_
#define _UML_INTERFACE_H_

#include "classifier.h"
#include "orderedSet.h"

namespace UML {
    class Interface : public Classifier {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;
        friend class Operation;

        protected:
            OrderedSet<Property, Interface> m_ownedAttributes = OrderedSet<Property, Interface>(this);
            OrderedSet<Operation, Interface> m_ownedOperations = OrderedSet<Operation, Interface>(this);
            OrderedSet<Classifier, Interface> m_nestedClassifiers = OrderedSet<Classifier, Interface>(this);
            Set<Property, Interface>& getOwnedAttributesSet();
            Set<Operation, Interface>& getOwnedOperationsSet();
            Set<Classifier, Interface>& getNestedClassifiersSet();
            void init();
            Interface();
        public:
            virtual ~Interface();
            OrderedSet<Property, Interface>& getOwnedAttributes();
            OrderedSet<Operation, Interface>& getOwnedOperations();
            OrderedSet<Classifier, Interface>& getNestedClassifiers();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INTERFACE_UML;
            }
    };
}

#endif
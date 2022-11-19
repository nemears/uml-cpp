#ifndef _UML_INTERFACE_H_
#define _UML_INTERFACE_H_

#include "classifier.h"
#include "set/orderedSet.h"

namespace UML {
    class Interface : public Classifier {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;
        friend class Operation;

        protected:
            CustomOrderedSet<Property, Interface> m_ownedAttributes = CustomOrderedSet<Property, Interface>(this);
            CustomOrderedSet<Operation, Interface> m_ownedOperations = CustomOrderedSet<Operation, Interface>(this);
            CustomOrderedSet<Classifier, Interface> m_nestedClassifiers = CustomOrderedSet<Classifier, Interface>(this);
            // Set<Property, Interface>& getOwnedAttributesSet();
            // Set<Operation, Interface>& getOwnedOperationsSet();
            // Set<Classifier, Interface>& getNestedClassifiersSet();
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
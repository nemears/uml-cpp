#pragma once

#include "encapsulatedClassifier.h"
#include "uml/set/orderedSet.h"
#include "behavioredClassifier.h"

namespace UML{

    class Operation;
    class Reception;
    class Property;

    class Class : public EncapsulatedClassifier , public BehavioredClassifier {

        template <class SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;
        friend class Operation;

        protected:
            IndexableOrderedSet<Property, Class> m_classOwnedAttrubutes = IndexableOrderedSet<Property, Class>(this);
            IndexableOrderedSet<Operation, Class> m_ownedOperations = IndexableOrderedSet<Operation, Class>(this);
            IndexableOrderedSet<Classifier, Class> m_nestedClassifiers = IndexableOrderedSet<Classifier, Class>(this);
            IndexableSet<Reception, Class> m_ownedReceptions = IndexableSet<Reception, Class>(this);
            void restoreReferences() override;
            Class();
        public:
            virtual ~Class();
            IndexableOrderedSet<Property, Class>& getOwnedAttributes();
            IndexableOrderedSet<Operation, Class>& getOwnedOperations();
            IndexableOrderedSet<Classifier, Class>& getNestedClassifiers();
            IndexableSet<Reception, Class>& getOwnedReceptions();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASS;
            };
    };
}

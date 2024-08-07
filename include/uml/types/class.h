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
            OrderedSet<Property, Class> m_classOwnedAttrubutes = OrderedSet<Property, Class>(this);
            OrderedSet<Operation, Class> m_ownedOperations = OrderedSet<Operation, Class>(this);
            OrderedSet<Classifier, Class> m_nestedClassifiers = OrderedSet<Classifier, Class>(this);
            Set<Reception, Class> m_ownedReceptions = Set<Reception, Class>(this);
            void restoreReferences() override;
            Class();
        public:
            virtual ~Class();
            OrderedSet<Property, Class>& getOwnedAttributes();
            OrderedSet<Operation, Class>& getOwnedOperations();
            OrderedSet<Classifier, Class>& getNestedClassifiers();
            Set<Reception, Class>& getOwnedReceptions();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASS;
            };
    };
}

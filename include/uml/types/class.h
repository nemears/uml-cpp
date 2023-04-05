#ifndef _UML_CLASS_H_
#define _UML_CLASS_H_

#include "encapsulatedClassifier.h"
#include "operation.h"
#include "uml/set/orderedSet.h"
#include "behavioredClassifier.h"
#include "reception.h"

namespace UML{

    class Operation;
    class Reception;

    class Class : public EncapsulatedClassifier , public BehavioredClassifier {

        template <class SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;
        friend class Operation;

        protected:
            CustomOrderedSet<Property, Class> m_classOwnedAttrubutes = CustomOrderedSet<Property, Class>(this);
            CustomOrderedSet<Operation, Class> m_ownedOperations = CustomOrderedSet<Operation, Class>(this);
            CustomOrderedSet<Classifier, Class> m_nestedClassifiers = CustomOrderedSet<Classifier, Class>(this);
            CustomSet<Reception, Class> m_ownedReceptions = CustomSet<Reception, Class>(this);
            void restoreReference(Element* el) override;
            Class();
        public:
            virtual ~Class();
            OrderedSet<Property, Class>& getOwnedAttributes();
            OrderedSet<Operation, Class>& getOwnedOperations();
            OrderedSet<Classifier, Class>& getNestedClassifiers();
            Set<Reception, Class>& getOwnedReceptions();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASS;
            };
    };
}

#endif
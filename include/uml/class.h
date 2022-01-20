#ifndef _CLASS_H_
#define _CLASS_H_

#include "encapsulatedClassifier.h"
#include "operation.h"
#include "parameter.h"
#include "orderedSet.h"
#include "behavioredClassifier.h"

namespace UML{

    class Operation;

    class Class : public EncapsulatedClassifier , public BehavioredClassifier {

        friend class UmlManager;
        friend class Property;
        friend class Operation;

        protected:
            OrderedSet<Property, Class> m_classOwnedAttrubutes = OrderedSet<Property, Class>(this);
            OrderedSet<Operation, Class> m_ownedOperations = OrderedSet<Operation, Class>(this);
            OrderedSet<Classifier, Class> m_nestedClassifiers = OrderedSet<Classifier, Class>(this);
            Set<Property, Class>& getOwnedAttributesSet();
            Set<Operation, Class>& getOwnedOperationsSet();
            Set<Classifier, Class>& getNestedClassifiersSet();
            void init();
            void copy(const Class& rhs);
            Class();
        public:
            virtual ~Class();
            Class(const Class& clazz);
            OrderedSet<Property, Class>& getOwnedAttributes();
            OrderedSet<Operation, Class>& getOwnedOperations();
            OrderedSet<Classifier, Class>& getNestedClassifiers();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASS;
            };
    };
}

#endif
#ifndef _UML_CLASS_H_
#define _UML_CLASS_H_

#include "encapsulatedClassifier.h"
#include "operation.h"
#include "orderedSet.h"
#include "behavioredClassifier.h"
#include "reception.h"

namespace UML{

    class Operation;
    class Reception;

    class Class : public EncapsulatedClassifier , public BehavioredClassifier {

        friend class UmlManager;
        friend class Property;
        friend class Operation;

        protected:
            OrderedSet<Property, Class> m_classOwnedAttrubutes = OrderedSet<Property, Class>(this);
            OrderedSet<Operation, Class> m_ownedOperations = OrderedSet<Operation, Class>(this);
            OrderedSet<Classifier, Class> m_nestedClassifiers = OrderedSet<Classifier, Class>(this);
            Set<Reception, Class> m_ownedReceptions = Set<Reception, Class>(this);
            Set<Property, Class>& getOwnedAttributesSet();
            Set<Operation, Class>& getOwnedOperationsSet();
            Set<Classifier, Class>& getNestedClassifiersSet();
            void init();
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
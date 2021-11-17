#ifndef CLASSH
#define CLASSH
#include <list>
#include "structuredClassifier.h"
// #include "behavioredClassifier.h"

namespace UML{

    class Operation;

    class Class : public StructuredClassifier /**, public BehavioredClassifier**/ {

        friend class UmlManager;
        friend class Property;

        protected:
            Set<Property, Class> m_classOwnedAttrubutes = Set<Property, Class>(this);
        //     Sequence<Operation> m_ownedOperations = Sequence<Operation>(this);
        //     Sequence<Classifier> m_nestedClassifiers = Sequence<Classifier>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Class& rhs);
            Class();
        public:
            virtual ~Class();
            Class(const Class& clazz);
            Set<Property, Class>& getOwnedAttributes();
            // Sequence<Operation>& getOwnedOperations();
            // Sequence<Classifier>& getNestedClassifiers();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CLASS;
            };
    };
}

#endif
#pragma once

#include "encapsulatedClassifier.h"
#include "uml/managers/abstractManager.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/orderedSet.h"
#include "behavioredClassifier.h"
#include "uml/types/element.h"

namespace UML{

    class Operation;
    class Reception;
    class Property;

    class Class : public EncapsulatedClassifier , public BehavioredClassifier {

        friend class Property;
        friend class Operation;
        friend struct ElementInfo<Class>;

        protected:
            IndexableOrderedSet<Property, Class> m_classOwnedAttrubutes = IndexableOrderedSet<Property, Class>(this);
            IndexableOrderedSet<Operation, Class> m_ownedOperations = IndexableOrderedSet<Operation, Class>(this);
            IndexableOrderedSet<Classifier, Class> m_nestedClassifiers = IndexableOrderedSet<Classifier, Class>(this);
            IndexableSet<Reception, Class> m_ownedReceptions = IndexableSet<Reception, Class>(this);
        public:
            Class(std::size_t elementType, AbstractManager& manager);
            IndexableOrderedSet<Property, Class>& getOwnedAttributes();
            IndexableOrderedSet<Operation, Class>& getOwnedOperations();
            IndexableOrderedSet<Classifier, Class>& getNestedClassifiers();
            IndexableSet<Reception, Class>& getOwnedReceptions();
            typedef TypeInfo<std::tuple<EncapsulatedClassifier, BehavioredClassifier>, Class> Info;
    };

    template <>
    struct ElementInfo<Class> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Class"; }
        static SetList sets(Class& el) {
            return SetList {
                makeSetPair("ownedAttributes", el.m_classOwnedAttrubutes),
                makeSetPair("ownedOperations", el.m_ownedOperations),
                makeSetPair("nestedClassifiers", el.m_nestedClassifiers),
                makeSetPair("ownedReceptions", el.m_ownedReceptions)
            };
        }
    };
}

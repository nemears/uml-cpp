#pragma once

#include "classifier.h"
#include "relationship.h"
#include "uml/set/indexableSet.h"
#include "uml/set/orderedSet.h"

namespace UML {
    class Association : public Classifier, public Relationship {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;

        protected:
            class MemberEndPolicy : public IndexablePolicy {
                protected:
                    void elementAdded(Property& el, Association& me);
                    void elementRemoved(Property& el, Association& me);
            };
            IndexableOrderedSet<Property, Association, MemberEndPolicy> m_memberEnds = IndexableOrderedSet<Property, Association, MemberEndPolicy>(this);
            IndexableOrderedSet<Property, Association> m_ownedEnds = IndexableOrderedSet<Property, Association>(this);
            IndexableSet<Property, Association> m_navigableOwnedEnds = IndexableSet<Property, Association>(this);
            IndexableSet<Type, Association> m_endTypes = IndexableSet<Type, Association>(this);
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Association();
        public:
            virtual ~Association();
            IndexableOrderedSet<Property, Association, MemberEndPolicy>& getMemberEnds();
            IndexableOrderedSet<Property, Association>& getOwnedEnds();
            IndexableSet<Property, Association>& getNavigableOwnedEnds();
            /**
             * endType is derived from the types of the member ends.
             **/
            IndexableSet<Type, Association>& getEndTypes();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ASSOCIATION;
            };
    };
}

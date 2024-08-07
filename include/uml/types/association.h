#pragma once

#include "classifier.h"
#include "relationship.h"
#include "uml/set/orderedSet.h"

namespace UML {
    class Association : public Classifier, public Relationship {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;

        protected:
            struct MemberEndPolicy {
                void elementAdded(Property& el, Association& me);
                void elementRemoved(Property& el, Association& me);
            };
            class RemoveMemberEndPolicy {
                public:
                    static void apply(Property& el, Association& me);
            };
            OrderedSet<Property, Association, MemberEndPolicy> m_memberEnds = OrderedSet<Property, Association, MemberEndPolicy>(this);
            OrderedSet<Property, Association> m_ownedEnds = OrderedSet<Property, Association>(this);
            Set<Property, Association> m_navigableOwnedEnds = Set<Property, Association>(this);
            Set<Type, Association> m_endTypes = Set<Type, Association>(this);
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Association();
        public:
            virtual ~Association();
            OrderedSet<Property, Association, MemberEndPolicy>& getMemberEnds();
            OrderedSet<Property, Association>& getOwnedEnds();
            Set<Property, Association>& getNavigableOwnedEnds();
            /**
             * endType is derived from the types of the member ends.
             **/
            Set<Type, Association>& getEndTypes();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ASSOCIATION;
            };
    };
}

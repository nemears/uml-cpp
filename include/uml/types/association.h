#pragma once

#include "classifier.h"
#include "relationship.h"
#include "uml/set/orderedSet.h"

namespace UML {
    class Association : public Classifier, public Relationship {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;
        friend class Property;

        protected:
            class AddMemberEndPolicy {
                public:
                    static void apply(Property& el, Association& me);
            };
            class RemoveMemberEndPolicy {
                public:
                    static void apply(Property& el, Association& me);
            };
            CustomOrderedSet<Property, Association, AddMemberEndPolicy, RemoveMemberEndPolicy> m_memberEnds = CustomOrderedSet<Property, Association, AddMemberEndPolicy, RemoveMemberEndPolicy>(this);
            CustomOrderedSet<Property, Association> m_ownedEnds = CustomOrderedSet<Property, Association>(this);
            CustomSet<Property, Association> m_navigableOwnedEnds = CustomSet<Property, Association>(this);
            CustomSet<Type, Association> m_endTypes = CustomSet<Type, Association>(this);
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Association();
        public:
            virtual ~Association();
            OrderedSet<Property, Association>& getMemberEnds();
            OrderedSet<Property, Association>& getOwnedEnds();
            Set<Property, Association>& getNavigableOwnedEnds();
            /**
             * endType is derived from the types of the member ends.
             **/
            Set<Type, Association>& getEndTypes();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ASSOCIATION;
            };
    };
}
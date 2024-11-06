#pragma once

#include "classifier.h"
#include "relationship.h"
#include "uml/managers/typeInfo.h"
#include "uml/set/indexableSet.h"
#include "uml/set/orderedSet.h"
#include "uml/types/element.h"

namespace UML {
    class Association : public Classifier, public Relationship {

        friend class Property;
        friend struct ElementInfo<Association>;

        protected:
            class MemberEndPolicy : public IndexablePolicy {
                protected:
                    void elementAdded(Property& el, Association& me);
                    void elementRemoved(Property& el, Association& me);
            };
            IndexableOrderedSet<Property, Association, MemberEndPolicy> m_memberEnds = IndexableOrderedSet<Property, Association, MemberEndPolicy>(this);
            IndexableOrderedSet<Property, Association> m_ownedEnds = IndexableOrderedSet<Property, Association>(this);
            IndexableSet<Property, Association> m_navigableOwnedEnds = IndexableSet<Property, Association>(this);
            ReadOnlyIndexableSet<Type, Association> m_endTypes = ReadOnlyIndexableSet<Type, Association>(this);
        public:
            Association(std::size_t elementType, AbstractManager& el);
            IndexableOrderedSet<Property, Association, MemberEndPolicy>& getMemberEnds();
            IndexableOrderedSet<Property, Association>& getOwnedEnds();
            IndexableSet<Property, Association>& getNavigableOwnedEnds();
            /**
             * endType is derived from the types of the member ends.
             **/
            ReadOnlyIndexableSet<Type, Association>& getEndTypes();
            typedef TypeInfo<std::tuple<Classifier, Relationship>, Association> Info;
    };

    template <>
    struct ElementInfo<Association> : public DefaultInfo {
        static const bool abstract = false;
        static std::string name(__attribute__((unused)) AbstractElement& el) { return "Association"; }
        static SetList sets(Association& el) {
            return SetList {
                makeSetPair("memberEnds", el.m_memberEnds),
                makeSetPair("ownedEnds", el.m_ownedEnds),
                makeSetPair("navigableOwnedEnds", el.m_navigableOwnedEnds),
                makeSetPair("endTypes",el.m_endTypes)
            };
        }
    };
}

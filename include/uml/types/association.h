#pragma once

#include "egm/egm-basic.h"
#include "uml/util/indexableSet.h"

namespace UML {
    template <class>
    class Classifier;

    template <class>
    class Relationship;

    template <class>
    class Property;

    template <class>
    class Type;

    template <class ManagerPolicy>
    class Association : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Association, EGM::TemplateTypeList<Classifier, Relationship>>;
        protected:
            using OwnedEnds = IndexableSet<Property, Association>;
            using NavigableOwnedEnds = IndexableSet<Property, Association>;
            struct MemberEndPolicy {
                using PropertyImpl = typename OwnedEnds::ManagedType;
                void elementAdded(PropertyImpl& el, Association& me) {
                    if (el.getType()) {
                        if (!me.getEndTypes().contains(el.getType())) {
                            me.addToReadonlySet(me.getEndTypes(), *el.getType());
                        }
                    }
                }
                void elementRemoved(PropertyImpl& el, Association& me) {
                    if (el.getType()) {
                        if (me.getEndTypes().contains(el.getType())) {
                            me.removeFromReadonlySet(me.getEndTypes(), *el.getType());
                        }
                    }
                }
            };
            using MemberEnds = IndexableSet<Property, Association, MemberEndPolicy>;
            using EndTypes = ReadOnlyIndexableSet<Type, Association>;
            MemberEnds m_memberEnds = MemberEnds(this);
            OwnedEnds m_ownedEnds = OwnedEnds(this);
            NavigableOwnedEnds m_navigableOwnedEnds = NavigableOwnedEnds(this);
            EndTypes m_endTypes = EndTypes(this);
        private:
            void init() {
                m_memberEnds.subsets(ManagerPolicy::m_members);
                m_memberEnds.opposite(&MemberEnds::ManagedType::getAssociationSingleton);
                m_ownedEnds.subsets(m_memberEnds);
                m_ownedEnds.subsets(ManagerPolicy::m_ownedMembers);
                m_ownedEnds.subsets(ManagerPolicy::m_features);
                m_navigableOwnedEnds.subsets(m_ownedEnds);
                m_ownedEnds.opposite(&OwnedEnds::ManagedType::getOwningAssociationSingleton);
                m_endTypes.subsets(ManagerPolicy::m_relatedElements);
            }
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Association);

            MemberEnds& getMemberEnds() { return m_memberEnds; }
            OwnedEnds& getOwnedEnds() { return m_ownedEnds; }
            NavigableOwnedEnds& getNavigableOwnedEnds() { return m_navigableOwnedEnds; }
            EndTypes& getEndTypes() { return m_endTypes; }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Association> {
        static std::string name() { return "Association"; }
        template <class Policy>
        static SetList sets(UML::Association<Policy>& el) {
            return SetList {
                make_set_pair("memberEnds", el.getMemberEnds()),
                make_set_pair("ownedEnds", el.getOwnedEnds()),
                make_set_pair("navigableOwnedEnds", el.getNavigableOwnedEnds()),
                make_set_pair("endTypes", el.getEndTypes())
            };
        }
    };
}

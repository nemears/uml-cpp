#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "classifier.h"
#include "relationship.h"
#include "orderedSet.h"

namespace UML {
    class Association : public Classifier, public Relationship {

        friend class UmlManager;
        friend class Property;

        protected:
            OrderedSet<Property, Association> m_memberEnds = OrderedSet<Property, Association>(this);
            OrderedSet<Property, Association> m_ownedEnds = OrderedSet<Property, Association>(this);
            Set<Property, Association> m_navigableOwnedEnds = Set<Property, Association>(this);
            Set<Type, Association> m_endType = Set<Type, Association>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void restoreReference(Element* el) override;
            void referenceErased(ID id) override;
            Set<Property, Association>& getMemberEndsSet();
            Set<Property, Association>& getOwnedEndsSet();
            void init();
            void copy(const Association& rhs);
            Association();
        public:
            Association(const Association& rhs);
            virtual ~Association();
            OrderedSet<Property, Association>& getMemberEnds();
            OrderedSet<Property, Association>& getOwnedEnds();
            Set<Property, Association>& getNavigableOwnedEnds();
            /**
             * endType is derived from the types of the member ends.
             **/
            Set<Type, Association>& getEndType();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ASSOCIATION;
            };
    };
}

#endif
#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "classifier.h"
#include "relationship.h"

namespace UML {
    class Association : public Classifier, public Relationship {
        protected:
            Sequence<Property> m_memberEnds;
            Sequence<Property> m_ownedEnds;
            Sequence<Property> m_navigableOwnedEnds;
            Sequence<Type> m_endType;
            void reindexID(ID oldID, ID newID) override;
            // void reindexName(std::string oldName, std::string newName) override;
            class AddMemberEndFunctor : public AbstractSequenceFunctor {
                public:
                    AddMemberEndFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveMemberEndFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveMemberEndFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddOwnedEndFunctor : public AbstractSequenceFunctor {
                public:
                    AddOwnedEndFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveOwnedEndFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveOwnedEndFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddNavigableOwnedEndFunctor : public AbstractSequenceFunctor {
                public:
                    AddNavigableOwnedEndFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveNavigableOwnedEndFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveNavigableOwnedEndFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            /**
             * TODO Think about constructor Association(Property& end1, Property& end2) {
             *      // sequence stuff
             *      m_memberEnd->add(end1);
             *      m_memberEnd->add(end2);
             * }
             * The reason is because an association NEEDS 2 ends atleast
             **/
            Association();
            ~Association();
            Sequence<Property>& getMemberEnds();
            Sequence<Property>& getOwnedEnds();
            Sequence<Property>& getNavigableOwnedEnds();
            /**
             * endType is derived from the types of the member ends.
             **/
            Sequence<Type>& getEndType();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::ASSOCIATION;
            };
    };
}

#endif
#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "classifier.h"
#include "relationship.h"

namespace UML {
    class Association : public Classifier, public Relationship {
        protected:
            Sequence<Property>* m_memberEnds;
            Sequence<Property>* m_ownedEnds;
            Sequence<Property>* m_navigableOwnedEnds;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
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
            Association();
            ~Association();
            Sequence<Property>& getMemberEnds();
            Sequence<Property>& getOwnedEnds();
            Sequence<Property>& getNavigableOwnedEnds();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
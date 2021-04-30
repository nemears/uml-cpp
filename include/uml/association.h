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
        public:
            Association();
            ~Association();
            Sequence<Property>& getMemberEnds();
            Sequence<Property>& getOwnedEnds();
            Sequence<Property>& getNavigableOwnedEnds();
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType);
    };
}

#endif
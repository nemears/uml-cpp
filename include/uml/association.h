#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "classifier.h"
#include "relationship.h"

namespace UML {
    class Association : public Classifier, public Relationship {
        friend class UmlManager;
        protected:
            Sequence<Property> m_memberEnds;
            Sequence<Property> m_ownedEnds;
            Sequence<Property> m_navigableOwnedEnds;
            Sequence<Type> m_endType;
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void setManager(UmlManager* manager) override;
            void referenceReindexed(ID oldID, ID newID) override;
            class AddMemberEndFunctor : public TemplateAbstractSequenceFunctor<Property,Association> {
                public:
                    AddMemberEndFunctor(Association* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class RemoveMemberEndFunctor : public TemplateAbstractSequenceFunctor<Property,Association> {
                public:
                    RemoveMemberEndFunctor(Association* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class AddOwnedEndFunctor : public TemplateAbstractSequenceFunctor<Property,Association> {
                public:
                    AddOwnedEndFunctor(Association* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class RemoveOwnedEndFunctor : public TemplateAbstractSequenceFunctor<Property,Association> {
                public:
                    RemoveOwnedEndFunctor(Association* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class AddNavigableOwnedEndFunctor : public TemplateAbstractSequenceFunctor<Property,Association> {
                public:
                    AddNavigableOwnedEndFunctor(Association* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
            };
            class RemoveNavigableOwnedEndFunctor : public TemplateAbstractSequenceFunctor<Property,Association> {
                public:
                    RemoveNavigableOwnedEndFunctor(Association* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Property& el) const override;
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
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ASSOCIATION;
            };
    };
}

#endif
#ifndef ASSOCIATION_H
#define ASSOCIATION_H

#include "classifier.h"
#include "relationship.h"

namespace UML {
    class Association : public Classifier, public Relationship {
        friend class UmlManager;
        protected:
            Sequence<Property> m_memberEnds = Sequence<Property>(this);
            Sequence<Property> m_ownedEnds = Sequence<Property>(this);
            Sequence<Property> m_navigableOwnedEnds = Sequence<Property>(this);
            Sequence<Type> m_endType = Sequence<Type>(this);
            void reindexID(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
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
            class AddEndTypeFunctor : public TemplateAbstractSequenceFunctor<Type,Association> {
                public:
                    AddEndTypeFunctor(Association* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Type& el) const override;
            };
            class RemoveEndTypeFunctor : public TemplateAbstractSequenceFunctor<Type,Association> {
                public:
                    RemoveEndTypeFunctor(Association* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Type& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            /**
             * TODO Think about constructor Association(Property& end1, Property& end2) {
             *      // sequence stuff
             *      m_memberEnd->add(end1);
             *      m_memberEnd->add(end2);
             * }
             * The reason is because an association NEEDS 2 ends atleast
             **/
            Association();
        public:
            Association(const Association& rhs);
            virtual ~Association();
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
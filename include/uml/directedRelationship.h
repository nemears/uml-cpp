#ifndef DIRECTEDRELATIONSHIP_H
#define DIRECTEDRELATIONSHIP_H

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        friend class UmlManager;
        protected:
            Sequence<> m_targets;
            Sequence<> m_sources;
            // below is commented out because should be obsolete with function pointer add and remove functionality on sequence?
            //void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;

            // functor triggered on getTargets().add(el) and getSources().add(el)
            class AddRelatedElementFunctor : public AbstractSequenceFunctor {
                public:
                    AddRelatedElementFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveRelatedElementFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveRelatedElementFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            DirectedRelationship();
            virtual ~DirectedRelationship();
            Sequence<>& getTargets();
            Sequence<>& getSources();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::DIRECTED_RELATIONSHIP;
            };
    };
}

#endif
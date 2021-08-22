#ifndef DIRECTEDRELATIONSHIP_H
#define DIRECTEDRELATIONSHIP_H

#include "relationship.h"

namespace UML {
    class DirectedRelationship : public Relationship {
        friend class UmlManager;
        protected:
            Sequence<> m_targets = Sequence<>(this);
            Sequence<> m_sources = Sequence<>(this);

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
            class AddSourcesFunctor : public AbstractSequenceFunctor {
                public:
                    AddSourcesFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveSourcesFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveSourcesFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddTargetsFunctor : public AbstractSequenceFunctor {
                public:
                    AddTargetsFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveTargetsFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveTargetsFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            DirectedRelationship();
            DirectedRelationship(const DirectedRelationship& relationship);
            virtual ~DirectedRelationship();
            Sequence<>& getTargets();
            Sequence<>& getSources();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DIRECTED_RELATIONSHIP;
            };
    };
}

#endif
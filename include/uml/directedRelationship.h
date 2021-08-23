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
            class AddRelatedElementFunctor : public TemplateAbstractSequenceFunctor<Element,DirectedRelationship> {
                public:
                    AddRelatedElementFunctor(DirectedRelationship* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveRelatedElementFunctor : public TemplateAbstractSequenceFunctor<Element,DirectedRelationship> {
                public:
                    RemoveRelatedElementFunctor(DirectedRelationship* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddSourcesFunctor : public TemplateAbstractSequenceFunctor<Element,DirectedRelationship> {
                public:
                    AddSourcesFunctor(DirectedRelationship* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveSourcesFunctor : public TemplateAbstractSequenceFunctor<Element,DirectedRelationship> {
                public:
                    RemoveSourcesFunctor(DirectedRelationship* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddTargetsFunctor : public TemplateAbstractSequenceFunctor<Element,DirectedRelationship> {
                public:
                    AddTargetsFunctor(DirectedRelationship* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveTargetsFunctor : public TemplateAbstractSequenceFunctor<Element,DirectedRelationship> {
                public:
                    RemoveTargetsFunctor(DirectedRelationship* me) : TemplateAbstractSequenceFunctor(me) {};
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
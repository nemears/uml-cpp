#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "behavior.h"
#include "activityNode.h"
#include "activityEdge.h"

namespace UML {
    class Activity : public Behavior {
        protected:
            Sequence<ActivityNode> m_nodes;
            Sequence<ActivityEdge> m_edges;
            class AddNodeFunctor : public TemplateAbstractSequenceFunctor<ActivityNode,Activity> {
                public:
                    AddNodeFunctor(Activity* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityNode& el) const override;
            };
            class AddEdgeFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge,Activity> {
                public:
                    AddEdgeFunctor(Activity* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
        public:
            Activity();
            ~Activity();
            Sequence<ActivityNode>& getNodes();
            Sequence<ActivityEdge>& getEdges();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY;
            };
    };
}


#endif
#ifndef ACTIVITY_H
#define ACTIVITY_H
#include "behavior.h"

namespace UML {

    class ActivityNode;
    class ActivityEdge;

    class Activity : public Behavior {

        friend class UmlManager;

        protected:
            Sequence<ActivityNode> m_nodes = Sequence<ActivityNode>(this);
            Sequence<ActivityEdge> m_edges = Sequence<ActivityEdge>(this);
            class AddNodeFunctor : public TemplateAbstractSequenceFunctor<ActivityNode,Activity> {
                public:
                    AddNodeFunctor(Activity* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityNode& el) const override;
            };
            class RemoveNodeFunctor : public TemplateAbstractSequenceFunctor<ActivityNode, Activity> {
                public:
                    RemoveNodeFunctor(Activity* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityNode& el) const override;
            };
            class AddEdgeFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge,Activity> {
                public:
                    AddEdgeFunctor(Activity* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            class RemoveEdgeFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge, Activity> {
                public:
                    RemoveEdgeFunctor(Activity* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
        public:
            Activity();
            Activity(const Activity& rhs);
            virtual ~Activity();
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
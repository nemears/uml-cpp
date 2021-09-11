#ifndef ACTIVITYEDGE_H
#define ACTIVITYEDGE_H

#include "redefinableElement.h"
#include "valueSpecification.h"
#include "sequence.h"

namespace UML {

    // forward declaration
    class ActivityNode;
    class Activity;

    class ActivityEdge : public RedefinableElement {

        friend class Activity;

        protected:
            Singleton<Activity, ActivityEdge> m_activity = Singleton<Activity, ActivityEdge>(this);
            class RemoveActivityProcedure : public AbstractSingletonProcedure<Activity, ActivityEdge> {
                public:
                    RemoveActivityProcedure(ActivityEdge* me) : AbstractSingletonProcedure<Activity, ActivityEdge>(me) {};
                    void operator()(Activity* el) const override;
            };
            class AddActivityProcedure : public AbstractSingletonProcedure<Activity, ActivityEdge> {
                public:
                    AddActivityProcedure(ActivityEdge* me) : AbstractSingletonProcedure<Activity, ActivityEdge>(me) {};
                    void operator()(Activity* el) const override;
             };
            Singleton<ActivityNode, ActivityEdge> m_source = Singleton<ActivityNode, ActivityEdge>(this);
            class RemoveSourceProcedure : public AbstractSingletonProcedure<ActivityNode, ActivityEdge> {
                public:
                    RemoveSourceProcedure(ActivityEdge* me) : AbstractSingletonProcedure<ActivityNode, ActivityEdge>(me) {};
                    void operator()(ActivityNode* el) const override;
            };
            class AddSourceProcedure : public AbstractSingletonProcedure<ActivityNode, ActivityEdge> {
                public:
                    AddSourceProcedure(ActivityEdge* me) : AbstractSingletonProcedure<ActivityNode, ActivityEdge>(me) {};
                    void operator()(ActivityNode* el) const override;
            };
            Singleton<ActivityNode, ActivityEdge> m_target = Singleton<ActivityNode, ActivityEdge>(this);
            class RemoveTargetProcedure : public AbstractSingletonProcedure<ActivityNode, ActivityEdge> {
                public:
                    RemoveTargetProcedure(ActivityEdge* me) : AbstractSingletonProcedure<ActivityNode, ActivityEdge>(me) {};
                    void operator()(ActivityNode* el) const override;
            };
            class AddTargetProcedure : public AbstractSingletonProcedure<ActivityNode, ActivityEdge> {
                public:
                    AddTargetProcedure(ActivityEdge* me) : AbstractSingletonProcedure<ActivityNode, ActivityEdge>(me) {};
                    void operator()(ActivityNode* el) const override;
            };
            ValueSpecification* m_guard;
            void reindexID(ID oldID, ID newID) override;
            // void reindexName(std::string oldName, std::string newName) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            ActivityEdge();
            ActivityEdge(const ActivityEdge& rhs);
            virtual ~ActivityEdge();
            Activity* getActivity();
            Activity& getActivityRef();
            bool hasActivity() const;
            void setActivity(Activity& activity);
            void setActivity(Activity* activity);
            ActivityNode* getSource();
            ActivityNode& getSourceRef();
            bool hasSource() const;
            void setSource(ActivityNode* source);
            void setSource(ActivityNode& source);
            ActivityNode* getTarget();
            ActivityNode& getTargetRef();
            bool hasTarget() const;
            void setTarget(ActivityNode* target);
            void setTarget(ActivityNode& target);
            ValueSpecification* getGuard();
            void setGuard(ValueSpecification* guard);
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_EDGE;
            };
    };
}

#endif
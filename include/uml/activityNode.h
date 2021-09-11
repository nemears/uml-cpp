#ifndef ACTIVITYNODE_H
#define ACTIVITYNODE_H

#include "redefinableElement.h"

namespace UML {

    class Activity;
    class ActivityEdge;

    class ActivityNode : virtual public RedefinableElement {

        friend class UmlManager;

        protected:
            Singleton<Activity, ActivityNode> m_activity = Singleton<Activity, ActivityNode>(this);
            class RemoveActivityProcedure : public AbstractSingletonProcedure<Activity, ActivityNode> {
                public:
                    RemoveActivityProcedure(ActivityNode* me) : AbstractSingletonProcedure<Activity, ActivityNode>(me) {};
                    void operator()(Activity* el) const override;
            };
            class AddActivityProcedure : public AbstractSingletonProcedure<Activity, ActivityNode> {
                public:
                    AddActivityProcedure(ActivityNode* me) : AbstractSingletonProcedure<Activity, ActivityNode>(me) {};
                    void operator()(Activity* el) const override;
            };
            Sequence<ActivityEdge> m_incoming = Sequence<ActivityEdge>(this);
            Sequence<ActivityEdge> m_outgoing = Sequence<ActivityEdge>(this);
            void reindexID(ID oldID, ID newID) override;
            // void reindexName(std::string oldName, std::string newName) override;
            class AddIncomingFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge,ActivityNode> {
                public:
                    AddIncomingFunctor(ActivityNode* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            class RemoveIncomingFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge, ActivityNode> {
                public:
                    RemoveIncomingFunctor(ActivityNode* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            class AddOutgoingFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge,ActivityNode> {
                public:
                    AddOutgoingFunctor(ActivityNode* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            class RemoveOutgoingFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge, ActivityNode> {
                public:
                    RemoveOutgoingFunctor(ActivityNode* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            class CheckIncomingFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge,ActivityNode> {
                public:
                    CheckIncomingFunctor(ActivityNode* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            class CheckOutgoingFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge,ActivityNode> {
                public:
                    CheckOutgoingFunctor(ActivityNode* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            void setManager(UmlManager* manager) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
        public:
            ActivityNode();
            ActivityNode(const ActivityNode& rhs);
            virtual ~ActivityNode();
            Sequence<ActivityEdge>& getIncoming();
            Sequence<ActivityEdge>& getOutgoing();
            ElementType getElementType() const override;
            Activity* getActivity();
            Activity& getActivityRef();
            bool hasActivity() const;
            void setActivity(Activity* activity);
            void setActivity(Activity& activity);
            virtual bool isObjectNode();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_NODE;
            };
    };

    // Errors

    class DuplicateEdgeException: public std::exception {
        private:
            std::string msg;
        public:
            DuplicateEdgeException(std::string uuid) :
                msg("Duplicate edge (uuid: " + uuid + ") added to sequence specified as set!")
                {};
            virtual const char* what() const throw() {
                return msg.c_str();
            };
    };
}

#endif
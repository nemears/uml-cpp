#ifndef ACTIVITYNODE_H
#define ACTIVITYNODE_H

#include "namedElement.h"
#include "activityEdge.h"

namespace UML {

    class Activity;

    class ActivityNode : virtual public NamedElement {
        protected:
            Activity* m_activity;
            Sequence<ActivityEdge> m_incoming;
            Sequence<ActivityEdge> m_outgoing;
            void reindexID(ID oldID, ID newID) override;
            // void reindexName(std::string oldName, std::string newName) override;
            class AddIncomingFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge,ActivityNode> {
                public:
                    AddIncomingFunctor(ActivityNode* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(ActivityEdge& el) const override;
            };
            class AddOutgoingFunctor : public TemplateAbstractSequenceFunctor<ActivityEdge,ActivityNode> {
                public:
                    AddOutgoingFunctor(ActivityNode* me) : TemplateAbstractSequenceFunctor(me) {};
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
        public:
            ActivityNode();
            ~ActivityNode();
            Sequence<ActivityEdge>& getIncoming();
            Sequence<ActivityEdge>& getOutgoing();
            ElementType getElementType() const override;
            Activity* getActivity();
            void setActivity(Activity* activity);
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
#ifndef ACTIVITYNODE_H
#define ACTIVITYNODE_H

#include "namedElement.h"
#include "activityEdge.h"

namespace UML {

    class Activity;

    class ActivityNode : virtual public NamedElement {
        protected:
            Activity* m_activity;
            Sequence<ActivityEdge>* m_incoming;
            Sequence<ActivityEdge>* m_outgoing;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
            class AddIncomingFunctor : public AbstractSequenceFunctor {
                public:
                    AddIncomingFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddOutgoingFunctor : public AbstractSequenceFunctor {
                public:
                    AddOutgoingFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class CheckIncomingFunctor : public AbstractSequenceFunctor {
                public:
                    CheckIncomingFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class CheckOutgoingFunctor : public AbstractSequenceFunctor {
                public:
                    CheckOutgoingFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            ActivityNode();
            ~ActivityNode();
            Sequence<ActivityEdge>& getIncoming();
            Sequence<ActivityEdge>& getOutgoing();
            ElementType getElementType() override;
            Activity* getActivity();
            void setActivity(Activity* activity);
            virtual bool isObjectNode();
    };

    // Errors

    class DuplicateEdgeException: public exception {
        private:
            string msg;
        public:
            DuplicateEdgeException(string uuid) :
                msg("Duplicate edge (uuid: " + uuid + ") added to sequence specified as set!")
                {};
            virtual const char* what() const throw() {
                return msg.c_str();
            };
    };
}

#endif
#ifndef ACTIVITYEDGE_H
#define ACTIVITYEDGE_H

#include "namedElement.h"
#include "valueSpecification.h"
#include "sequence.h"

namespace UML {

    // forward declaration
    class ActivityNode;
    class Activity;

    class ActivityEdge : public NamedElement {
        protected:
            Activity* m_activity;
            ActivityNode* m_source;
            ActivityNode* m_target;
            ValueSpecification* m_guard;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(std::string oldName, std::string newName) override;
        public:
            Activity* getActivity();
            void setActivity(Activity* activity);
            ActivityNode* getSource();
            void setSource(ActivityNode* source);
            ActivityNode* getTarget();
            void setTarget(ActivityNode* target);
            ValueSpecification* getGuard();
            void setGuard(ValueSpecification* guard);
            ActivityEdge() {
                m_activity = 0;
                m_source = 0;
                m_target = 0;
                m_guard = 0;
            }
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) override;
            static ElementType elementType() {
                return ElementType::ACTIVITY_EDGE;
            };
    };
}

#endif
#ifndef _UML_CALL_ACTION_H_
#define _UML_CALL_ACTION_H_

#include "invocationAction.h"

namespace UML {
    class CallAction : public InvocationAction {
        protected:
            OrderedSet<OutputPin, CallAction> m_results = OrderedSet<OutputPin, CallAction>(this);
            bool m_isSynchronous = true;
            Set<OutputPin, CallAction>& getResultsSet();
            void init();
            CallAction();
        public:
            virtual ~CallAction();
            OrderedSet<OutputPin, CallAction>& getResults();
            bool isSynchronous() const;
            void setIsSynchronous(bool val);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::CALL_ACTION;
            };
    };
}

#endif
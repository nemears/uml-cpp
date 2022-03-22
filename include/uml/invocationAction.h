#ifndef _UML_INVOCATION_ACTION_H_
#define _UML_INVOCATION_ACTION_H_

#include "action.h"
#include "orderedSet.h"

namespace UML {

    typedef UmlPtr<Port> PortPtr;

    class InvocationAction : public Action {
        protected:
            OrderedSet<InputPin, InvocationAction> m_arguments = OrderedSet<InputPin, InvocationAction>(this);
            Singleton<Port, InvocationAction> m_onPort = Singleton<Port, InvocationAction>(this);
            Set<InputPin, InvocationAction>& getArgumentsSet();
            Set<Port, InvocationAction>& getOnPortSingleton();
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(ID id, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
            InvocationAction();
        public:
            virtual ~InvocationAction();
            OrderedSet<InputPin, InvocationAction>& getArguments();
            PortPtr getOnPort() const;
            void setOnPort(Port* port);
            void setOnPort(Port& port);
            void setOnPort(PortPtr port);
            void setOnPort(ID id);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::INVOCATION_ACTION;
            };
    };
}

#endif
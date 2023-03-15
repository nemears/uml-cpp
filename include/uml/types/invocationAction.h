#ifndef _UML_INVOCATION_ACTION_H_
#define _UML_INVOCATION_ACTION_H_

#include "action.h"
#include"uml/set/orderedSet.h"

namespace UML {

    typedef UmlPtr<Port> PortPtr;

    class InvocationAction : public Action {
        protected:
            CustomOrderedSet<InputPin, InvocationAction> m_arguments = CustomOrderedSet<InputPin, InvocationAction>(this);
            CustomSingleton<Port, InvocationAction> m_onPort = CustomSingleton<Port, InvocationAction>(this);
            TypedSet<InputPin, InvocationAction>& getArgumentsSet();
            TypedSet<Port, InvocationAction>& getOnPortSingleton();
            void referenceReindexed(ID newID) override;
            void referenceErased(ID id) override;
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
#ifndef _UML_OPAQUE_ACTION_H_
#define _UML_OPAQUE_ACTION_H_

#include "action.h"
#include "literalString.h"
#include "set/orderedSet.h"

namespace UML {
    class OpaqueAction : public Action {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            CustomSet<InputPin, OpaqueAction> m_inputValues = CustomSet<InputPin, OpaqueAction>(this);
            CustomSet<OutputPin, OpaqueAction> m_outputValues = CustomSet<OutputPin, OpaqueAction>(this);
            CustomOrderedSet<LiteralString, OpaqueAction> m_bodies = CustomOrderedSet<LiteralString, OpaqueAction>(this); // TODO change to sequence when we make that
            OpaqueAction();
        public:
            virtual ~OpaqueAction();
            Set<InputPin, OpaqueAction>& getInputValues();
            Set<OutputPin, OpaqueAction>& getOutputValues();
            OrderedSet<LiteralString, OpaqueAction>& getBodies(); // todo sequence
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OPAQUE_ACTION;
            };
    };
}

#endif
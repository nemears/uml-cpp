#ifndef _UML_OPAQUE_ACTION_H_
#define _UML_OPAQUE_ACTION_H_

#include "action.h"
#include "literalString.h"
#include "orderedSet.h"

namespace UML {
    class OpaqueAction : public Action {

        friend class UmlManager;

        protected:
            Set<InputPin, OpaqueAction> m_inputValues = Set<InputPin, OpaqueAction>(this);
            Set<OutputPin, OpaqueAction> m_outputValues = Set<OutputPin, OpaqueAction>(this);
            OrderedSet<LiteralString, OpaqueAction> m_bodies = OrderedSet<LiteralString, OpaqueAction>(this); // TODO change to sequence when we make that
            Set<LiteralString, OpaqueAction>& getBodiesSet();
            void init();
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
#ifndef _UML_OUTPUT_PIN_H_
#define _UML_OUTPUT_PIN_H_

#include "pin.h"
#include "action.h"

namespace UML {
    class OutputPin : public Pin {

        template <typename AccessPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            OutputPin();
        public:
            virtual ~OutputPin();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OUTPUT_PIN;
            };
    };
}

#endif
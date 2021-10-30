#ifndef OUTPUTPIN_H
#define OUTPUTPIN_H

#include "pin.h"
#include "action.h"

namespace UML {
    class OutputPin : public Pin {

        friend class UmlManager;

        protected:
            // void reindexName(std::string oldName, std::string newName) override;
            OutputPin();
        public:
            OutputPin(const OutputPin& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::OUTPUT_PIN;
            };
    };
}

#endif
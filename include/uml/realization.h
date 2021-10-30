#ifndef REALIZATION_H
#define REALIZATION_H

#include "uml/abstraction.h"

namespace UML {
    /**
     * Realization is a specialized Abstraction dependency between two sets of NamedElements, one representing a 
     * specification (the supplier) and the other representing an implementation of that specification (the client).
     **/
    class Realization : public Abstraction {
        
        friend class UmlManager;

        protected:
            Realization();
        public:
            Realization(const Realization& realization);
            virtual ~Realization();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REALIZATION;
            };
    };
}

#endif
#pragma once

#include "abstraction.h"

namespace UML {
    /**
     * Realization is a specialized Abstraction dependency between two sets of NamedElements, one representing a 
     * specification (the supplier) and the other representing an implementation of that specification (the client).
     **/
    class Realization : public Abstraction {
        
        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Realization();
        public:
            virtual ~Realization();
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REALIZATION;
            };
    };
}

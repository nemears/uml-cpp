#ifndef _UML_REALIZATION_H_
#define _UML_REALIZATION_H_

#include "abstraction.h"

namespace UML {
    /**
     * Realization is a specialized Abstraction dependency between two sets of NamedElements, one representing a 
     * specification (the supplier) and the other representing an implementation of that specification (the client).
     **/
    class Realization : public Abstraction {
        
        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Realization();
        public:
            virtual ~Realization();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REALIZATION;
            };
    };
}

#endif
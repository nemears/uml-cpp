#ifndef _UML_ABSTRACTION_H_
#define _UML_ABSTRACTION_H_

#include "dependency.h"

namespace UML {
    /**
     * An Abstraction is a Dependency that relates two NamedElements or sets of NamedElements that represent the same 
     * concept at different levels of abstraction or from different viewpoints.
     **/
    class Abstraction : public Dependency {

        template <typename AccessPolicy, typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            Abstraction();
        public:
            Abstraction(const Abstraction& abstraction);
            virtual ~Abstraction();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::ABSTRACTION;
            };
    };
}

#endif
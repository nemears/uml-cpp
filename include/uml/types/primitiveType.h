#ifndef _UML_PRIMITIVE_TYPE_H_
#define _UML_PRIMITIVE_TYPE_H_

#include "dataType.h"

namespace UML{
    class PrimitiveType : public DataType {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            PrimitiveType();
        public:
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PRIMITIVE_TYPE;
            };
    };
}
#endif
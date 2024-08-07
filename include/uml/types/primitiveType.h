#pragma once

#include "dataType.h"

namespace UML{
    class PrimitiveType : public DataType {

        template <typename SerializationPolicy, typename PersistencePolicy> friend class Manager;

        protected:
            PrimitiveType();
        public:
            bool is(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::PRIMITIVE_TYPE;
            };
    };
}

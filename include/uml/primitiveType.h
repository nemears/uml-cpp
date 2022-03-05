#ifndef _UML_PRIMITIVE_TYPE_H_
#define _UML_PRIMITIVE_TYPE_H_

#include "dataType.h"

namespace UML{
    class PrimitiveType : public DataType {

        friend class UmlManager;

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
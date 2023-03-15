#ifndef _UML_TYPE_H_
#define _UML_TYPE_H_

#include "packageableElement.h"
#include "package.h"

namespace UML{

    class TypedElement;
    class Association;
    class Operation;

    class Type : virtual public PackageableElement {

        friend class TypedElement;
        friend class Association;
        friend class Operation;
        
        protected:
            Type();
        public:
            virtual ~Type();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::TYPE;
            };
    };
}

#endif
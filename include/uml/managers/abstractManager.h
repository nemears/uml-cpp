#ifndef _UML_MANAGERS_ABSTRACT_MANAGER_H_
#define _UML_MANAGERS_ABSTRACT_MANAGER_H_

#include "uml/element.h"

namespace UML {
    class AbstractManager {
        public:
            virtual Element* create(ElementType type) = 0;
            template <class T = Element>
            UmlPtr<T> create() {
                return UmlPtr<T>(&create(T::elementType())->template as<T>());
            }
            virtual ElementPtr get(ID id) = 0;
    };
}

#endif
#ifndef MODELH
#define MODELH

#include "package.h"

namespace UML{
    class Model : public Package {

        friend class UmlManager;

        protected:
            Model();
        public:
            Model(const Model& rhs);
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::MODEL;
            };
    };
}
#endif
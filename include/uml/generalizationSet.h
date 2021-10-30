#ifndef UML_GENERALIZATION_SET_H
#define UML_GENERALIZATION_SET_H

#include "uml/packageableElement.h"

namespace UML {
    class GeneralizationSet : public PackageableElement {

        friend class UmlManager;

        protected:
            GeneralizationSet();
        public:
    };
}

#endif
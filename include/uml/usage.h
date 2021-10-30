#ifndef USAGE_H
#define USAGE_H

#include "dependency.h"

namespace UML {
    /**
     * A Usage is a Dependency in which one NamedElement requires another NamedElement (or set of NamedElements) for 
     * its full implementation or operation.
     **/
    class Usage : public Dependency {

        friend class UmlManager;

        protected:
            Usage();
        public:
            Usage(const Usage& usage);
            virtual ~Usage();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::USAGE;
            };
    };
}

#endif
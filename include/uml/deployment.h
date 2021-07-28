#ifndef DEPLOYMENT_H
#define DEPLOYMENT_H

#include "dependency.h"

namespace UML {
    class Deployment : public Dependency {
        public:
            Deployment();
            Deployment(const Deployment& deployment);
            virtual ~Deployment();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT;
            };
    };
}

#endif
#ifndef DEPLOYMENT_TARGET_H
#define DEPLOYMENT_TARGET_H

#include "namedElement.h"

namespace UML {
    class DeploymentTarget : public NamedElement {
        private:
        public:
            DeploymentTarget();
            DeploymentTarget(const DeploymentTarget& deploymentTarget);
            virtual ~DeploymentTarget();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::DEPLOYMENT_TARGET;
            };
    };
}

#endif
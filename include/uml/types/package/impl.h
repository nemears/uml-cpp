#pragma once

#include "definition.h"
#include "uml/types/packageableElement/definition.h"

namespace UML {
    template <class ManagerPolicy>
    class Package : public PackageDefinition<ManagerPolicy> {
        protected:
            Set<PackageableElementDefinition, PackageDefinition<ManagerPolicy>> m_packagedElements = Set<PackageableElementDefinition, PackageDefinition<ManagerPolicy>>(this);
        public:
            Set<PackageableElementDefinition, PackageDefinition<ManagerPolicy>>& getPackagedElements() override {
                return m_packagedElements;
            }
    };
}

#pragma once

#include "uml/managers/manager.h"
#include "serverPersistencePolicy.h"

namespace UML {
    class UmlClient : public Manager<OpenUmlJsonSerializationPolicy, ServerPersistencePolicy> {
        public:
            ElementPtr get(std::string qualifiedName);
            ElementPtr get(ID id) override;
            void setRoot(Element* root) override;
    };
}
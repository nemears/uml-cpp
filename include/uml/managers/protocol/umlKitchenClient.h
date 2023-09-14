#pragma once

#include "uml/managers/manager.h"
#include "umlKitchenPersistencePolicy.h"
//#include "uml/managers/serialization/open_uml/openUmlSerializationPolicy.h"

namespace UML {
    class UmlKitchenClient : public Manager<OpenUmlJsonSerializationPolicy, UmlKitchenPersistencePolicy> {
        public:
            UmlKitchenClient();
            UmlKitchenClient(std::string address, std::string server, std::string user, std::string passwordHash);
    };
}

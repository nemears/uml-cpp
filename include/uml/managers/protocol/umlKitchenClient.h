#pragma once

#include "uml/managers/manager.h"
#include "umlKitchenPersistencePolicy.h"
#include <string>
#include <websocketpp/config/asio_client.hpp>

namespace UML {
    class UmlKitchenClient : public Manager<OpenUmlJsonSerializationPolicy, UmlKitchenPersistencePolicy<websocketpp::config::asio_tls_client>> {
        public:
            UmlKitchenClient();
            UmlKitchenClient(std::string address, std::string server, std::string user, std::string passwordHash);
            void init(std::string address, std::string project, std::string user, std::string passwordHash) override;
    };
}

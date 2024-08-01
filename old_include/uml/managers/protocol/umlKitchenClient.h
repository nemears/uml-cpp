#pragma once

#include "uml/managers/manager.h"
#include "uml/managers/serialization/open_uml/openUmlSerializationPolicy.h"
#include "umlKitchenPersistencePolicy.h"
#include <string>
#include <websocketpp/config/asio_client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

namespace UML {
    class UmlKitchenClient : public Manager<OpenUmlJsonSerializationPolicy, UmlKitchenPersistencePolicy<websocketpp::config::asio_tls_client>> {
        public:
            UmlKitchenClient();
            UmlKitchenClient(std::string address, std::string project, std::string user, std::string passwordHash);
            void init(std::string address, std::string project, std::string user, std::string passwordHash) override;
    };

    class NoTlsUmlKitchenClient : public Manager<OpenUmlJsonSerializationPolicy, UmlKitchenPersistencePolicy<websocketpp::config::asio_client>> {
        public:
            NoTlsUmlKitchenClient();
            NoTlsUmlKitchenClient(std::string addres, std::string project, std::string user, std::string passwordHash);
    };
}

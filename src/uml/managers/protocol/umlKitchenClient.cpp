#include "uml/managers/protocol/umlKitchenPersistencePolicy.h"
#include <uml/uml-stable.h>
#include <uml/managers/protocol/umlKitchenClient.h>
#include <websocketpp/common/memory.hpp>
#include <websocketpp/config/asio_client.hpp>

using namespace UML;

websocketpp::lib::shared_ptr<asio::ssl::context> handleTLS() {
    websocketpp::lib::shared_ptr<asio::ssl::context> context = websocketpp::lib::make_shared<asio::ssl::context>(asio::ssl::context::sslv23);
    try {
        context->set_options(asio::ssl::context::default_workarounds |
                         asio::ssl::context::no_sslv2 |
                         asio::ssl::context::no_sslv3 |
                         asio::ssl::context::single_dh_use);
    } catch (std::exception& e) {
        std::cout << "could not set tls options for client" << std::endl;
    }
    return context;
}

UmlKitchenClient::UmlKitchenClient() {
    // Do nothing :)
}

UmlKitchenClient::UmlKitchenClient(std::string address, std::string project, std::string user, std::string passwordHash) {
    login(address, project, user, passwordHash);
}

void UmlKitchenClient::init(std::string address, std::string project, std::string user, std::string passwordHash) {
   UmlKitchenPersistencePolicy<websocketpp::config::asio_tls_client>::init(address, project, user, passwordHash);
   m_websocketClient.set_tls_init_handler(websocketpp::lib::bind(&handleTLS));
}

NoTlsUmlKitchenClient::NoTlsUmlKitchenClient() {

}

NoTlsUmlKitchenClient::NoTlsUmlKitchenClient(std::string address, std::string project, std::string user, std::string passwordHash) {
    login(address, project, user, passwordHash);
}

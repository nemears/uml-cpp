#include "uml/managers/protocol/umlKitchenPersistencePolicy.h"
#include <uml/uml-stable.h>
#include <uml/managers/protocol/umlKitchenClient.h>
#include <websocketpp/common/memory.hpp>
#include <websocketpp/config/asio_client.hpp>

using namespace UML;

websocketpp::lib::shared_ptr<boost::asio::ssl::context> handleTLS() {
    websocketpp::lib::shared_ptr<boost::asio::ssl::context> context = websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::sslv23);
    try {
        context->set_options(boost::asio::ssl::context::default_workarounds |
                         boost::asio::ssl::context::no_sslv2 |
                         boost::asio::ssl::context::no_sslv3 |
                         boost::asio::ssl::context::single_dh_use);
    } catch (std::exception& e) {
        std::cout << "could not set tls options for client" << std::endl;
    }
    return context;
}

UmlKitchenClient::UmlKitchenClient() {
    // Do nothing :)
}

UmlKitchenClient::UmlKitchenClient(std::string address, std::string server, std::string user, std::string passwordHash) {
    login(address, server, user, passwordHash);
}

void UmlKitchenClient::init(std::string address, std::string project, std::string user, std::string passwordHash) {
   UmlKitchenPersistencePolicy<websocketpp::config::asio_tls_client>::init(address, project, user, passwordHash);
   m_websocketClient.set_tls_init_handler(websocketpp::lib::bind(&handleTLS));
}

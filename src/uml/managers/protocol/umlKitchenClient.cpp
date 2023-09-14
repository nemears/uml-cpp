#include <uml/uml-stable.h>
#include <uml/managers/protocol/umlKitchenClient.h>

using namespace UML;

UmlKitchenClient::UmlKitchenClient() {
    // Do nothing :)
}

UmlKitchenClient::UmlKitchenClient(std::string address, std::string server, std::string user, std::string passwordHash) {
    login(address, server, user, passwordHash);
}

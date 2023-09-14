#include "uml/managers/protocol/umlKitchenPersistencePolicy.h"
#include "uml/managers/manager.h"
#include "yaml-cpp/node/parse.h"
#include <string>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/logger/levels.hpp>
#include <websocketpp/roles/client_endpoint.hpp>
#include <yaml-cpp/yaml.h>

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

namespace UML {

void handleInitialization(
        UmlKitchenPersistencePolicy* me, 
        websocketpp::connection_hdl hdl, 
        websocketpp::client<websocketpp::config::asio_client>::message_ptr msg
) {
    YAML::Node node = YAML::Load(msg->get_payload());
    if (node["client"] && ID::fromString(node["client"].as<std::string>()) == me->m_clientID) {
        me->m_readonly = node["edit"].as<bool>();
        std::unique_lock<std::mutex> lck(me->m_initializedMtx);
        me->m_initialized = true;
        me->m_initializedCV.notify_all();
    } else {
        std::cout << "bad initialization message from server" << std::endl;
    }
}

void handleOpen(UmlKitchenPersistencePolicy* me, websocketpp::connection_hdl hdl) {
    websocketpp::lib::error_code initEc;
    me->m_websocketClient.send(
            hdl, 
            "{\"server\":\"" + me->m_project + "\",\"id\":\"" + me->m_clientID.string() + "\",\"user\":\"" + me->m_user + "\",\"paswordHash\":\"" + me->m_passwordHash + "\"}", 
            websocketpp::frame::opcode::text, 
            initEc
    );
    if (initEc) {
        std::cout << "could not send initialization message to server because: " << initEc.message() << std::endl;
    }
 
}

void UmlKitchenPersistencePolicy::login(std::string address, std::string project, std::string user, std::string passwordHash) {
    m_address = address;
    m_project = project;
    m_user = user;
    m_passwordHash = passwordHash;
    m_websocketClient.set_access_channels(websocketpp::log::alevel::all);
    m_websocketClient.clear_access_channels(websocketpp::log::alevel::frame_payload);
    m_websocketClient.init_asio();
    m_websocketClient.set_message_handler(websocketpp::lib::bind(&handleInitialization, this, ::_1, ::_2));
    m_websocketClient.set_open_handler(websocketpp::lib::bind(&handleOpen, this, ::_1));
    websocketpp::lib::error_code ec;
    websocketpp::client<websocketpp::config::asio_client>::connection_ptr con = m_websocketClient.get_connection(address, ec);
    if (ec) {
        std::cout << "could not create connection because: " << ec.message() << std::endl;
    }
    m_websocketClient.connect(con);
    m_websocketClient.run();
    
    std::unique_lock<std::mutex> lck(m_initializedMtx);
    while (!m_initialized) {
        m_initializedCV.wait(lck);
    }
}

std::string UmlKitchenPersistencePolicy::loadElementData(ID id) {
    
}

std::string UmlKitchenPersistencePolicy::getProjectData(std::string path) {
    // TODO connect to new server and get head
    throw ManagerStateException("Do not try to open with a client, get individual elements");
}

std::string UmlKitchenPersistencePolicy::getProjectData() {
    // get head and construct ????
    throw ManagerStateException("Do not try to open with a client, get individual elements");
}
}

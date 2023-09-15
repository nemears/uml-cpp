#include "uml/managers/protocol/umlKitchenPersistencePolicy.h"
#include "uml/managers/manager.h"
#include "yaml-cpp/node/parse.h"
#include <mutex>
#include <string>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/common/functional.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/frame.hpp>
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

void handleGetResponse(
        UmlKitchenPersistencePolicy* me, 
        websocketpp::connection_hdl hdl, 
        websocketpp::client<websocketpp::config::asio_client>::message_ptr msg
) {
    // TODO communicate string to other thread   
    std::unique_lock<std::mutex> lck(me->m_responseMtx);
    me->m_response = msg->get_payload();
    me->m_responseUpdated = true;
    me->m_responseCV.notify_one();
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
    m_connection = m_websocketClient.get_connection(address, ec);
    if (ec) {
        std::cout << "could not create connection because: " << ec.message() << std::endl;
    }
    m_websocketClient.connect(m_connection);
    m_websocketClient.run();
    
    std::unique_lock<std::mutex> lck(m_initializedMtx);
    while (!m_initialized) {
        m_initializedCV.wait(lck);
    }
}

std::string UmlKitchenPersistencePolicy::loadElementData(ID id) {
    // lock scope for request
    std::lock_guard<std::mutex> requestLck(m_requestMtx);

    // set response
    m_websocketClient.set_message_handler(websocketpp::lib::bind(&handleGetResponse, this, ::_1, ::_2));

    // request
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "GET" << YAML::Value << id.string() << 
    YAML::EndMap;
    websocketpp::lib::error_code ec;
    m_websocketClient.send(m_connection, emitter.c_str(), websocketpp::frame::opcode::text, ec);
    
    if (ec) {
        std::cout << "could not request data of element with id  " << id.string() << " from uml-kitchen instance!" <<  std::endl; 
    }

    // wait for response
    std::unique_lock<std::mutex> lck(m_responseMtx);
    while (!m_responseUpdated) {
        m_responseCV.wait(lck);
    }
    m_responseUpdated = false;
    return m_response;
}

void UmlKitchenPersistencePolicy::saveElementData(std::string data, ID id) {
    // lock scope for request
    std::lock_guard<std::mutex> requestLck(m_requestMtx);
    
    // send request
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "PUT" << YAML::Value << YAML::BeginMap << YAML::Key << 
        "id" << YAML::Value << id.string() << YAML::Key << "element" << YAML::Value << 
        YAML::Load(data) << YAML::EndMap << YAML::EndMap;
    websocketpp::lib::error_code ec;
    m_websocketClient.send(m_connection, emitter.c_str(), websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "could not send element with id " << id.string() << " to uml-kitchen instance!" << std::endl;
    }
}

std::string UmlKitchenPersistencePolicy::getProjectData(std::string path) {
    // TODO connect to new server and get head
    throw ManagerStateException("Do not try to open with a client, get individual elements");
}

std::string UmlKitchenPersistencePolicy::getProjectData() {
    // get head and construct ????
    throw ManagerStateException("Do not try to open with a client, get individual elements");
}

void UmlKitchenPersistencePolicy::saveProjectData(std::string data, std::string path) {
    websocketpp::lib::error_code ec;
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << YAML::Key << "save" << YAML::Value << "." << YAML::EndMap;
    m_websocketClient.send(m_connection, emitter.c_str(), websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "could not send project data to uml-kitchen instance!" << std::endl; 
    }
}

void UmlKitchenPersistencePolicy::saveProjectData(std::string data) {
    websocketpp::lib::error_code ec;
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << YAML::Key << "save" << YAML::Value << "." << YAML::EndMap;
    m_websocketClient.send(m_connection, emitter.c_str(), websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "could not send project data to uml-kitchen instance!" << std::endl; 
    }
}

void UmlKitchenPersistencePolicy::eraseEl(ID id) {
    websocketpp::lib::error_code ec;
    YAML::Emitter emitter;
    emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
        YAML::Key << "DELETE" << YAML::Value << id.string() << 
    YAML::EndMap;
    m_websocketClient.send(m_connection, emitter.c_str(), websocketpp::frame::opcode::text, ec);
    if (ec) {
        std::cout << "could not send erase request for element with id " << id.string() << " to  uml-kitchen instance!" << std::endl; 
    }
}

void UmlKitchenPersistencePolicy::reindex(ID oldID, ID newID) {

}
        
}

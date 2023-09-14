#pragma once

#include <condition_variable>
#include <uml/id.h>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/roles/client_endpoint.hpp>
#include "websocketpp/config/asio_client.hpp"
#include "websocketpp/client.hpp"

namespace UML {

    class UmlKitchenPersistencePolicy;

    void handleInitialization(
            UmlKitchenPersistencePolicy*, 
            websocketpp::connection_hdl, 
            websocketpp::client<websocketpp::config::asio_client>::message_ptr
    );

    void handleOpen(UmlKitchenPersistencePolicy*, websocketpp::connection_hdl);

    class UmlKitchenPersistencePolicy {

        friend void handleInitialization(
                UmlKitchenPersistencePolicy*, 
                websocketpp::connection_hdl,
                websocketpp::client<websocketpp::config::asio_client>::message_ptr 
        ); 
        friend void handleOpen(UmlKitchenPersistencePolicy*, websocketpp::connection_hdl);

        private:
            // websocket client object
            websocketpp::client<websocketpp::config::asio_client> m_websocketClient;
            
            // uml-kitchen info for server
            ID m_clientID = ID::randomID();
            std::string m_address = "";
            std::string m_project = "";
            std::string m_user = "";
            std::string m_passwordHash = "";
            bool m_readonly = false;

            // initialization blocking
            bool m_initialized = false;
            std::mutex m_initializedMtx;
            std::condition_variable m_initializedCV;
        protected:
            UmlKitchenPersistencePolicy();
            void login(std::string address, std::string server, std::string user, std::string passwordHash);
            std::string loadElementData(ID id);
            void saveElementData(std::string data, ID id);
            std::string getProjectData(std::string path);
            std::string getProjectData();
            void saveProjectData(std::string data, std::string path);
            void saveProjectData(std::string data);
            void eraseEl(ID id);
            void reindex(ID oldID, ID newID);

    };
}

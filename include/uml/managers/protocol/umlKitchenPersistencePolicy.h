#pragma once

#include <condition_variable>
#include <uml/id.h>
#include <websocketpp/common/connection_hdl.hpp>
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
    
    void handleGetResponse(
            UmlKitchenPersistencePolicy*, 
            websocketpp::connection_hdl, 
            websocketpp::client<websocketpp::config::asio_client>::message_ptr
    );

    class UmlKitchenPersistencePolicy {

        friend void handleInitialization(
                UmlKitchenPersistencePolicy*, 
                websocketpp::connection_hdl,
                websocketpp::client<websocketpp::config::asio_client>::message_ptr 
        );
        friend void handleGetResponse(
            UmlKitchenPersistencePolicy*, 
            websocketpp::connection_hdl, 
            websocketpp::client<websocketpp::config::asio_client>::message_ptr
        );
 
        friend void handleOpen(UmlKitchenPersistencePolicy*, websocketpp::connection_hdl);

        private:
            // websocket client and connection objects
            websocketpp::client<websocketpp::config::asio_client> m_websocketClient;
            websocketpp::client<websocketpp::config::asio_client>::connection_ptr m_connection = 0;

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

            // request handling blocking
            std::mutex m_requestMtx;
            std::string m_response = "";
            bool m_responseUpdated = false;
            std::mutex m_responseMtx;
            std::condition_variable m_responseCV;
        protected:
            // policy methods
            std::string loadElementData(ID id);
            void saveElementData(std::string data, ID id);
            std::string getProjectData(std::string path);
            std::string getProjectData();
            void saveProjectData(std::string data, std::string path);
            void saveProjectData(std::string data);
            void eraseEl(ID id);
            void reindex(ID oldID, ID newID);
        public:
            // login method (need to call this once to use client)
            void login(std::string address, std::string server, std::string user, std::string passwordHash);
    };
}

#pragma once

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>
#include <uml/id.h>
#include <websocketpp/close.hpp>
#include <yaml-cpp/yaml.h>
#include <websocketpp/common/connection_hdl.hpp>
#include <websocketpp/roles/client_endpoint.hpp>
#include "uml/managers/abstractManager.h"
#include "websocketpp/client.hpp"
#include <websocketpp/endpoint.hpp>

namespace UML {
    
    template <class WebsocketConfig>
    class UmlKitchenPersistencePolicy;
    
    template <class WebsocketConfig>
    void handleInitialization(
            UmlKitchenPersistencePolicy<WebsocketConfig>*, 
            websocketpp::connection_hdl, 
            typename  websocketpp::client<WebsocketConfig>::message_ptr
    );

    template <class WebsocketConfig>
    void handleOpen(UmlKitchenPersistencePolicy<WebsocketConfig>*, websocketpp::connection_hdl);
    
    template <class WebsocketConfig>
    void handleGetResponse(
            UmlKitchenPersistencePolicy<WebsocketConfig>*, 
            websocketpp::connection_hdl, 
            typename websocketpp::client<WebsocketConfig>::message_ptr
    );

    template <class WebsocketConfig>
    void handleClose(UmlKitchenPersistencePolicy<WebsocketConfig>*, websocketpp::connection_hdl);
    
    template <class WebsocketConfig>
    void handleFail(UmlKitchenPersistencePolicy<WebsocketConfig>*, websocketpp::connection_hdl);

    template <class WebsocketConfig>
    void backgroundThread(UmlKitchenPersistencePolicy<WebsocketConfig>*);

    template <class WebsocketConfig>
    class UmlKitchenPersistencePolicy {

        friend void handleInitialization<WebsocketConfig>(
                UmlKitchenPersistencePolicy<WebsocketConfig>*, 
                websocketpp::connection_hdl,
                typename websocketpp::client<WebsocketConfig>::message_ptr 
        );
        friend void handleGetResponse<WebsocketConfig>(
            UmlKitchenPersistencePolicy<WebsocketConfig>*, 
            websocketpp::connection_hdl, 
            typename websocketpp::client<WebsocketConfig>::message_ptr
        );
        friend void handleOpen<WebsocketConfig>(UmlKitchenPersistencePolicy<WebsocketConfig>*, websocketpp::connection_hdl);
        friend void backgroundThread<WebsocketConfig>(UmlKitchenPersistencePolicy<WebsocketConfig>*);
        friend void handleClose<WebsocketConfig>(UmlKitchenPersistencePolicy<WebsocketConfig>*, websocketpp::connection_hdl);
        friend void handleFail<WebsocketConfig>(UmlKitchenPersistencePolicy<WebsocketConfig>*, websocketpp::connection_hdl);

        private:
            std::thread* m_backgroundThread = 0;

            // initialization blocking
            bool m_initialized = false;
            bool m_badInitialization = false;
            std::mutex m_initializedMtx;
            std::condition_variable m_initializedCV;

            // request handling blocking
            std::mutex m_requestMtx;
            std::string m_response = "";
            bool m_responseUpdated = false;
            std::mutex m_responseMtx;
            std::condition_variable m_responseCV;
        protected:
            // websocket client and connection objects
            websocketpp::client<WebsocketConfig> m_websocketClient;
            typename websocketpp::client<WebsocketConfig>::connection_ptr m_connection = 0;

            // uml-kitchen info for server
            ID m_clientID = ID::randomID();
            std::string m_address = "";
            std::string m_project = "";
            std::string m_user = "";
            std::string m_passwordHash = "";
            bool m_readonly = false;

            // policy methods
            std::string loadElementData(ID id) {
               // lock scope for request
                std::lock_guard<std::mutex> requestLck(m_requestMtx);

                // set response
                m_connection->set_message_handler(websocketpp::lib::bind(&handleGetResponse<WebsocketConfig>, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));

                // request
                YAML::Emitter emitter;
                emitter << YAML::DoubleQuoted  << YAML::Flow << YAML::BeginMap << 
                YAML::Key << "GET" << YAML::Value << id.string() << 
                YAML::EndMap;
                websocketpp::lib::error_code ec;
                m_websocketClient.send(m_connection, emitter.c_str(), websocketpp::frame::opcode::text, ec);
                
                std::cout << "sent get request to client: " << emitter.c_str() << std::endl;
                if (ec) {
                    std::cout << "could not request data of element with id  " << id.string() << " from uml-kitchen instance!" <<  std::endl; 
                }

                // wait for response
                std::unique_lock<std::mutex> lck(m_responseMtx);
                while (!m_responseUpdated) {
                    m_responseCV.wait(lck);
                }
                m_responseUpdated = false;
                m_connection->set_message_handler(0);
                return m_response;
 
            }

            void saveElementData(std::string data, ID id) {
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
                std::cout << "sent message to server: " << emitter.c_str() << std::endl;
            }

            std::string getProjectData(std::string path) {
            
            }

            std::string getProjectData() {
            
            }

            void saveProjectData(std::string data, std::string path) {
                websocketpp::lib::error_code ec;
                YAML::Emitter emitter;
                emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << YAML::Key << "save" << YAML::Value << "." << YAML::EndMap;
                m_websocketClient.send(m_connection, emitter.c_str(), websocketpp::frame::opcode::text, ec);
                if (ec) {
                    std::cout << "could not send project data to uml-kitchen instance!" << std::endl; 
                }
            }

            void saveProjectData(std::string data) {
                websocketpp::lib::error_code ec;
                YAML::Emitter emitter;
                emitter << YAML::DoubleQuoted << YAML::Flow << YAML::BeginMap << YAML::Key << "save" << YAML::Value << "." << YAML::EndMap;
                m_websocketClient.send(m_connection, emitter.c_str(), websocketpp::frame::opcode::text, ec);
                if (ec) {
                    std::cout << "could not send project data to uml-kitchen instance!" << std::endl; 
                }
            }

            void eraseEl(ID id) {
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

            void reindex(ID oldID, ID newID) {
                // TODO
            }

            virtual void init(std::string address, std::string project, std::string user, std::string passwordHash) {
                m_address = address;
                m_project = project;
                m_user = user;
                m_passwordHash = passwordHash;
                m_websocketClient.set_access_channels(websocketpp::log::alevel::all);
                m_websocketClient.clear_access_channels(websocketpp::log::alevel::frame_payload);
                m_websocketClient.init_asio();
                m_websocketClient.set_message_handler(websocketpp::lib::bind(&handleInitialization<WebsocketConfig>, this, websocketpp::lib::placeholders::_1, websocketpp::lib::placeholders::_2));
                m_websocketClient.set_open_handler(websocketpp::lib::bind(&handleOpen<WebsocketConfig>, this, websocketpp::lib::placeholders::_1));
                m_websocketClient.set_close_handler(websocketpp::lib::bind(&handleClose<WebsocketConfig>, this, websocketpp::lib::placeholders::_1));
                m_websocketClient.set_fail_handler(websocketpp::lib::bind(&handleFail<WebsocketConfig>, this, websocketpp::lib::placeholders::_1));
            }
        public:
            virtual ~UmlKitchenPersistencePolicy() {
                if (m_initialized) {
                    m_websocketClient.close(m_connection, websocketpp::close::status::going_away, "");
                }
                if (m_backgroundThread) {
                    m_backgroundThread->join();
                    delete m_backgroundThread;
                }
            }
            // login method (need to call this once to use client)
            void login(std::string address, std::string project, std::string user, std::string passwordHash) {
                m_initialized = false;
                m_badInitialization = false;
                init(address, project, user, passwordHash);
                
                std::cout << "initialized UmlKitchenClient" << std::endl;

                websocketpp::lib::error_code ec;
                m_connection = m_websocketClient.get_connection(address, ec);
                if (ec) {
                    std::cout << "could not create connection because: " << ec.message() << std::endl;
                    return;
                }
                m_websocketClient.connect(m_connection);
                m_backgroundThread = new std::thread(backgroundThread<WebsocketConfig>, this);
                std::unique_lock<std::mutex> lck(m_initializedMtx);
                while (!m_initialized && !m_badInitialization) {
                    m_initializedCV.wait(lck);
                }
                if (m_badInitialization) {
                    m_badInitialization = false;
                    throw ManagerStateException("could not connect to server");
                }
                std::cout << "finished connecting to server" << std::endl; 
            }
    };
    
    template <class WebsocketConfig>
    void handleInitialization(
        UmlKitchenPersistencePolicy<WebsocketConfig>* me, 
        websocketpp::connection_hdl hdl, 
        typename websocketpp::client<WebsocketConfig>::message_ptr msg
    ) {
        if (me->m_initialized || me->m_badInitialization) {
            return;
        }
        YAML::Node node = YAML::Load(msg->get_payload());
        std::cout << "got response from server: " << msg->get_payload() << std::endl;
        if (node["client"] && ID::fromString(node["client"].as<std::string>()) == me->m_clientID) {
            me->m_readonly = node["edit"].as<bool>();
            std::cout << "received proper id response from server" << std::endl;
            std::unique_lock<std::mutex> lck(me->m_initializedMtx);
            me->m_initialized = true;
            me->m_initializedCV.notify_all();
        } else {
            std::cout << "bad initialization message from server" << std::endl;
            std::unique_lock<std::mutex> lck(me->m_initializedMtx);
            me->m_badInitialization = true;
            me->m_initializedCV.notify_all();
        }
        me->m_connection->set_message_handler(0);
    }

    template <class WebsocketConfig>
    void handleOpen(UmlKitchenPersistencePolicy<WebsocketConfig>* me, websocketpp::connection_hdl hdl) {
        websocketpp::lib::error_code initEc;
        std::string request = "{\"server\":\"" + me->m_user + "/" + me->m_project + "\",\"id\":\"" + me->m_clientID.string() + "\",\"user\":\"" + me->m_user + "\",\"passwordHash\":\"" + me->m_passwordHash + "\",\"create\":false}";  
        me->m_websocketClient.send(
            hdl, 
            request,
            websocketpp::frame::opcode::text, 
            initEc
        );
        if (initEc) {
            std::cout << "could not send initialization message to server because: " << initEc.message() << std::endl;
        }
        std::cout << "sent initialization request: " << request << std::endl;
    }

    template <class WebsocketConfig>
    void handleGetResponse(
        UmlKitchenPersistencePolicy<WebsocketConfig>* me, 
        typename websocketpp::connection_hdl hdl, 
        typename websocketpp::client<WebsocketConfig>::message_ptr msg
    ) {
        // TODO communicate string to other thread   
        std::unique_lock<std::mutex> lck(me->m_responseMtx);
        me->m_response = msg->get_payload();
        std::cout << "got response from server " << me->m_response  << std::endl;
        me->m_responseUpdated = true;
        me->m_responseCV.notify_one();
    }

    template <class WebsocketConfig>
    void backgroundThread(UmlKitchenPersistencePolicy<WebsocketConfig>* me) {
        me->m_websocketClient.run();
    }

    template <class WebsocketConfig>
    void handleClose(UmlKitchenPersistencePolicy<WebsocketConfig>* me, websocketpp::connection_hdl hdl) {
        std::unique_lock<std::mutex> lck(me->m_initializedMtx);
        me->m_badInitialization = true;
        me->m_initializedCV.notify_all();
    }

    template <class WebsocketConfig>
    void handleFail(UmlKitchenPersistencePolicy<WebsocketConfig>* me, websocketpp::connection_hdl hdl) {
        std::unique_lock<std::mutex> lck(me->m_initializedMtx);
        me->m_badInitialization = true;
        me->m_initializedCV.notify_all();
    }
}

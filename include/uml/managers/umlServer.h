#ifndef UML_UML_SERVER_H
#define UML_UML_SERVER_H

#include "uml/umlManager.h"
#include <atomic>
#include <iostream>
#include <mutex>
#include <condition_variable>

#define UML_PORT 8652
#define UML_SERVER_MSG_SIZE 200

namespace std {
    class thread;
}

namespace UML {

    class UmlServer : public UmlManager {
        private:
            struct ClientInfo {
                int socket;
                std::thread* thread;
            };

            //data
            const ID m_shutdownID = ID::randomID();
            int m_socketD = 0;
            std::unordered_map<ID, ClientInfo> m_clients;
            std::unordered_map<std::string, ID> m_urls;

            // threading
            static void acceptNewClients(UmlServer* me);
            static void receiveFromClient(UmlServer* me, ID id);
            std::thread* m_acceptThread;
            std::unordered_map<ID, std::mutex*> m_locks;
            std::atomic<bool> m_running = false;
            std::mutex m_runMtx;
            std::condition_variable m_runCv;
            std::mutex m_logMtx;
            std::mutex m_acceptMtx;
            std::mutex m_msgMtx;
            std::condition_variable m_msgCv;
            std::atomic<bool> m_msgV = true;

            // helper methods
            Element& post(ElementType eType);
            void createNode(Element* el) override;
            void log(std::string msg);
        public:
            UmlServer();
            virtual ~UmlServer();
            int numClients();
            bool loaded(ID id) override;
    };
}

#endif
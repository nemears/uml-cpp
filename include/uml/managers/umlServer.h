#ifndef UML_UML_SERVER_H
#define UML_UML_SERVER_H

#include "uml/umlManager.h"
#include <atomic>
#include <iostream>
#include <mutex>
#include <condition_variable>

#define UML_PORT 8652

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
            const ID m_shutdownID = ID::randomID();
            std::atomic<bool> m_running = false;
            std::mutex m_runMtx;
            std::condition_variable m_runCv;
            std::atomic<std::ostream*> m_stream = &std::cout;
            int m_socketD = 0;
            static void acceptNewClients(UmlServer* me);
            static void receiveFromClient(UmlServer* me, ID id);
            Element& post(ElementType eType);
            std::thread* m_acceptThread;
            std::unordered_map<ID, ClientInfo> m_clients;
            std::unordered_map<ID, std::mutex*> m_locks;
            void createNode(Element* el) override;
        public:
            UmlServer();
            virtual ~UmlServer();
            int numClients();
    };
}

#endif
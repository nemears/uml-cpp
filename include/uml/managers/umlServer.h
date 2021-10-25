#ifndef UML_UML_SERVER_H
#define UML_UML_SERVER_H

#include "uml/umlManager.h"
#include <atomic>
#include <iostream>
#include <mutex>

#define UML_PORT 8652

namespace std {
    class thread;
}

namespace UML {

    struct ServerNode : public ManagerNode {
        std::mutex mtx;
    };

    class UmlServer : public UmlManager {
        private:
            struct ClientInfo {
                int socket;
                std::thread* thread;
            };
            std::atomic<bool> m_running = false;
            std::atomic<std::ostream*> m_stream = &std::cout;
            int m_socketD = 0;
            static void acceptNewClients(UmlServer* me);
            static void receiveFromClient(UmlServer* me, ID id);
            Element& post(ElementType eType);
            std::thread* m_acceptThread;
            std::unordered_map<ID, ClientInfo> m_clients;
        public:
            UmlServer();
            virtual ~UmlServer();
            int numClients();
    };
}

#endif
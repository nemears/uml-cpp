#ifndef UML_UML_SERVER_H
#define UML_UML_SERVER_H

#include "uml/umlManager.h"
#include <atomic>
#include <iostream>

#define UML_PORT 8652

namespace std {
    class thread;
}

namespace UML {

    class UmlServer : public UmlManager {
        private:
            std::atomic<bool> m_running = false;
            std::atomic<std::ostream*> m_stream = &std::cout;
            int m_socketD = 0;
            static void acceptNewClients(UmlServer* me);
            std::thread* m_acceptThread;
            std::unordered_map<ID, int> m_clients;
        public:
            UmlServer();
            virtual ~UmlServer();
            int numClients();
    };
}

#endif
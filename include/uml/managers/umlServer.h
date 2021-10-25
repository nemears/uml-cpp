#ifndef UML_UML_SERVER_H
#define UML_UML_SERVER_H

#include "uml/umlManager.h"

#define UML_PORT 8652

namespace std {
    class thread;
}

namespace UML {

    class UmlServer : public UmlManager {
        private:
            int m_socketD = 0;
            static void acceptNewClients(UmlServer* me);
            std::thread* m_acceptThread;
            std::unordered_map<ID, int> m_clients;
        public:
            UmlServer();
            virtual ~UmlServer();
    };
}

#endif
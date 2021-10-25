#ifndef UML_UML_SERVER_H
#define UML_UML_SERVER_H

#include "uml/umlManager.h"

#define UML_PORT 8652

namespace UML {

    static void main(int argc, char* argv);

    class UmlServer : public UmlManager {
        private:
            int m_socketD = 0;
            static void acceptNewClients(UmlServer* me);
            std::unordered_map<ID, int> m_clients;
        public:
            UmlServer();
    };
}

#endif
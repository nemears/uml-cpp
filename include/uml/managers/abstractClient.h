#ifndef _UML_MANAGERS_ABSTRACT_CLIENT_H_
#define _UML_MANAGERS_ABSTRACT_CLIENT_H_

#include "uml/element.h"
#ifdef WIN32
#include "winsock2.h"
#include <ws2tcpip.h>
#include <stdio.h>
#endif

namespace UML {
    class AbstractClient {
        private:
            std::string m_address;
            int m_port;
        protected:
            #ifndef WIN32
            int m_socketD = 0;
            #else
            SOCKET m_socketD = INVALID_SOCKET;
            WSADATA m_wsaData;
            #endif
            #ifdef UML_DEBUG
            std::string time_in_HH_MM_SS_MMM();
            #endif
            void init();
            ElementPtr getElFromServer(ID id, AbstractManager* me);
            ElementPtr getElFromServer(std::string name, AbstractManager* me);
            void writeToServer(Element& el);
            void saveToServer();
            void saveToServer(std::string path);
            void eraseFromServer(Element& el);
            void createElForServer(Element& el);
            void setRootForServer(Element* el);
        public:
            const ID id;
            AbstractClient();
            AbstractClient(std::string m_address);
            virtual ~AbstractClient();
    };
}

#endif
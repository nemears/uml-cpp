#ifndef _UML_UML_CLIENT_H_
#define _UML_UML_CLIENT_H_

#include "managerPolicy.h"
#include "simpleAccessPolicy.h"
#include "serverPersistencePolicy.h"

#ifdef WIN32
#include "winsock2.h"
#include <ws2tcpip.h>
#include <stdio.h>
#endif

namespace UML {
    class UmlClient : public Manager<SimpleAccessPolicy, ServerPersistencePolicy> {
        private:
            std::string m_address;
            int m_port;
            #ifndef WIN32
            int m_socketD = 0;
            #else
            SOCKET m_socketD = INVALID_SOCKET;
            WSADATA m_wsaData;
            #endif
            void init();
        public:
            const ID id;
            UmlClient();
            UmlClient(std::string m_address);
            virtual ~UmlClient();
            ElementPtr get(ID id) override;
            // template <class T = Element> T& get(ID id) {
            //     return get(id).as<T>();
            // }
            ElementPtr get(std::string qualifiedName);
            // template <class T = Element> T& get(std::string qualifiedName) {
            //     return get(qualifiedName).as<T>();
            // }
            void put(Element& el);
            void putAll();
            Element& post(ElementType eType);
            template<class T = Element> 
            UmlPtr<T> create() {
                return UmlPtr<T>(&post(T::elementType()).template as<T>());
            }
            void erase(Element& el) override;
            // ElementPtr aquire(ID id) override;
            void release(Element& el) override;
            template <class ... Elements> void release(Element& el, Elements&... els) {
                release(el);
                release(els...);
            }
            // void release(ID id) override;
            void setRoot(Element* root) override;
            void shutdownServer(); // maybe we need to pass a key or something
            void save() override;
            void save(std::string path) override;
    };
}

#endif
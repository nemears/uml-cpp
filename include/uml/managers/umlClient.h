#ifndef UML_UML_CLIENT_H
#define UML_UML_CLIENT_H

#include "uml/umlManager.h"

namespace UML {
    class UmlClient : public UmlManager {
        private:
            std::string m_address;
            int m_port;
            int m_socketD = 0;
            void init();
        public:
            const ID id;
            UmlClient();
            UmlClient(std::string m_address);
            virtual ~UmlClient();
            Element& get(ID id);
            template <class T = Element> T& get(ID id) {
                return get(id).as<T>();
            };
            Element& get(std::string qualifiedName);
            template <class T = Element> T& get(std::string qualifiedName) {
                return get(qualifiedName).as<T>();
            };
            void put(Element& el);
            void putAll();
            Element& post(ElementType eType);
            template<class T = Element> T& post() {
                return post(T::elementType()).template as<T>();
            };
            void erase(Element& el) override;
            Element* aquire(ID id) override;
            void release(Element& el) override;
            template <class ... Elements> void release(Element& el, Elements&... els) {
                release(el);
                release(els...);
            };
            void release(ID id) override;
            void shutdownServer(); // maybe we need to pass a key or something
    };
}

#endif
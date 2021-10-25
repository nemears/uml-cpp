#ifndef UML_UML_CLIENT_H
#define UML_UML_CLIENT_H

#include "uml/umlManager.h"

namespace UML {
    class UmlClient : public UmlManager {
        private:
            int m_socketD = 0;
        public:
            const ID id;
            UmlClient();
            virtual ~UmlClient();
            Element& get(ID id);
            template <class T = Element> T& get(ID id) {
                return get(id).as<T>();
            };
            Element& get(std::string qualifiedName);
            void put(Element& el);
            Element& post(ElementType eType);
            template<class T = Element> T& post() {
                return post(T::elementType()).template as<T>();
            };
            void erase(Element& el);
    };
}

#endif
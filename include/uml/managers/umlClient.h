#ifndef UML_UML_CLIENT_H
#define UML_UML_CLIENT_H

#include "uml/element.h"

namespace UML {
    class UmlClient {
        private:
            int m_socketD = 0;
        public:
            const ID id;
            UmlClient();
            virtual ~UmlClient();
            Element& get(ID id);
            Element& get(std::string qualifiedName);
            void put(Element& el);
            template <class T = Element> T& post() {
                // TODO
            };
            void erase(Element& el);
    };
}

#endif
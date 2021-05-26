#ifndef UML_MANAGER_H
#define UML_MANAGER_H

#include <map>

namespace UML {

    class Element;

    class ID {
        private:
            uint8_t m_data[21];
            char base64(bool in[6]);
        public:
            ID();
            std::string string();
            static ID randomID();
    };

    class UmlManager {
        public:
            std::map<ID, Element> m_elements;
    };
}

#endif
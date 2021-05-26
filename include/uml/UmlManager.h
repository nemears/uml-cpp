#ifndef UML_MANAGER_H
#define UML_MANAGER_H

#include <map>
#include <boost/uuid/uuid.hpp>

namespace UML {

    class Element;

    class ID {
        private:
            uint8_t m_data[21];
            char base64(bool in[6]);
        public:
            ID();
            std::string string();
    };

    class UmlManager {
        public:
            std::map<boost::uuids::uuid, Element> m_elements;
    };
}

#endif
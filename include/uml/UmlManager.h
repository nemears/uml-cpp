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
            uint8_t* getBytes();
            void setBytes(uint8_t* bytes);
            friend bool operator==(const ID& lhs, const ID& rhs) {
                for (size_t i = 0; i < 21; i++) {
                    if (lhs.m_data[i] != rhs.m_data[i]) {
                        return false;
                    }
                }

                return true;
            };
            friend bool operator!=(const ID& lhs, const ID& rhs){
                for (size_t i = 0; i < 21; i++) {
                    if (lhs.m_data[i] != rhs.m_data[i]) {
                        return true;
                    }
                }

                return false;
            };
            static ID randomID();
            static ID fromString(std::string id);
    };

    class UmlManager {
        public:
            std::map<ID, Element> m_elements;
    };
}

#endif
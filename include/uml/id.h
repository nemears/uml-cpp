#ifndef UML_ID_H
#define UML_ID_H

#include <string>

namespace UML {
    class ID {

        friend struct std::hash<UML::ID>;

        private:
            uint8_t m_data[21];
            char base64(bool in[6]) const;
        public:
            ID();
            std::string string() const;
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
}

// hash operator injection
namespace std {
    template <> struct hash<UML::ID> {
        std::size_t operator()(const UML::ID& id) const {

            /**
             * TODO implement own bitwise hashing
             **/

            hash<string> hasher;
            return hasher(id.string());
        }
    };
}

#endif
#pragma once

#include <string>
#include "stdint.h"

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
            bool isNull() const;
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
            bool operator>(const ID& rhs);
            bool operator<(const ID& rhs);
            static ID nullID();
            static const ID randomID();
            static ID fromString(std::string id);
            static bool isValid(std::string id);
    };

    class InvalidUmlID_Exception : public std::exception {
        private:
            std::string m_msg;
        public:
            InvalidUmlID_Exception(std::string msg) : m_msg(msg) {};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            };
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

#pragma once
#include "uml/umlPtr.h"
#include "uml/managers/abstractManager.h"

namespace UML {

    enum class SerializationMode {
        INDIVIDUAL,
        WHOLE
    };

    class EmitterData {
        public:
            SerializationMode mode = SerializationMode::INDIVIDUAL;
            bool isJSON = false;
            bool emitReferences = false;
    };

    class ParserData {
        public:
            SerializationMode mode = SerializationMode::INDIVIDUAL;
            AbstractManager* manager = 0;
    };

    class OpenUmlSerializationPolicy {
        protected:
            std::string extensionName();
            ElementPtr parseIndividual(std::string data, AbstractManager& manager);
            ElementPtr parseWhole(std::string data, AbstractManager& manager);
            std::string emitIndividual(Element& el, AbstractManager& manager);
            std::string emitWhole(Element& el, AbstractManager& manager);
    };

    class EmitterData;
    class ParserData;

    std::string emit(Element& el, EmitterData& data);
    ElementPtr parse(std::string data, ParserData& metaData);

    class SerializationError : public std::exception {
        private:
            std::string m_msg;
        public:
            SerializationError(std::string msg) : m_msg(msg) {};
            virtual const char* what() const throw() {
                return m_msg.c_str();
            }
    };
}
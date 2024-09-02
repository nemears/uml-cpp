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
    };

    class ParserData {
        public:
            SerializationMode mode = SerializationMode::INDIVIDUAL;
            AbstractManager* manager = 0;
            bool update = false; // update flag
    };

    class UmlCafeSerializationPolicy {
        protected:
            std::string extensionName();
            AbstractElementPtr parseIndividual(std::string data, AbstractManager& manager);
            AbstractElementPtr parseWhole(std::string data, AbstractManager& manager);
            std::string emitIndividual(AbstractElement& el, AbstractManager& manager);
            std::string emitWhole(AbstractElement& el, AbstractManager& manager);
    };

    class UmlCafeJsonSerializationPolicy {
        protected:
            std::string extensionName();
            AbstractElementPtr parseIndividual(std::string data, AbstractManager& manager);
            AbstractElementPtr parseWhole(std::string data, AbstractManager& manager);
            std::string emitIndividual(AbstractElement& el, AbstractManager& manager);
            std::string emitWhole(AbstractElement& el, AbstractManager& manager);
    };
    class EmitterData;
    class ParserData;

    std::string emit(AbstractElement& el, EmitterData& data);
    AbstractElementPtr parse(std::string data, ParserData& metaData);

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

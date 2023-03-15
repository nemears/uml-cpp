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
            std::string path = "";
    };

    class OpenUmlSerializationPolicy {
        protected:
            std::string extensionName();
            ElementPtr parseIndividual(std::string data, AbstractManager& manager);
            ElementPtr parseWhole(std::string data, AbstractManager& manager);
            std::string emitIndividual(Element& el, AbstractManager& manager);
            std::string emitWhole(Element& el, AbstractManager& manager);
    };

    std::string emit(Element& el, EmitterData& data);

    class UmlParserException : public std::exception {
        private:
            std::string m_msg;

            public:
                UmlParserException(const std::string msg, const std::string path, size_t line) : 
                    m_msg(msg + " path " + path + " line number " + std::to_string(line))
                    {};
                UmlParserException(std::string msg, std::string path) :
                    m_msg(msg) 
                    {};
                UmlParserException(std::string msg) :
                    m_msg(msg) 
                    {};
                virtual const char* what() const throw() {
                    return m_msg.c_str();
                }
    };
}
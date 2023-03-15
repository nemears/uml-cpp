#ifndef _UML_PARSERS_PARSER_H_
#define _UML_PARSERS_PARSER_H_

#include <exception>
#include "yaml-cpp/yaml.h"
#include "parserMetaData.h"
#include "emitterMetaData.h"
#include "uml/umlPtr.h"
#include "uml/set/set.h"
#include "uml/set/singleton.h"
#include "uml/forwardDeclarations.h"

namespace UML {
    namespace Parsers {

        class UmlParserException : public std::exception {
            private:
            std::string m_msg;

            public:
                UmlParserException(const std::string msg, const std::string path, YAML::Node node) : 
                    m_msg(msg + " path " + path + " line number " + std::to_string(node.Mark().line))
                    {};
                UmlParserException(std::string msg, std::string path) :
                    m_msg(msg) 
                    {};
                virtual const char* what() const throw() {
                    return m_msg.c_str();
                }
        };

        ElementPtr parse(ParserMetaData& data);

        ElementPtr parseString(std::string body, ParserMetaData& data);
        ElementPtr parseYAML(YAML::Node node, ParserMetaData& data);

        EmitterMetaData getData(Element& el);
        std::string emit(Element& el);
        void emit(Element& el, YAML::Emitter& emitter);
        std::string emitIndividual(Element& el);
        void emitIndividual(Element& el, YAML::Emitter& emitter);
        std::string emitString(EmitterMetaData& data, Element& el);
        void emit(EmitterMetaData& data);

        // sets up yaml emitter for emitting to a new file and emits
        void emitToFile(Element& el, EmitterMetaData& data, std::string path, std::string fileName);

        namespace {
            
        }

        ElementType elementTypeFromString(std::string eType);
    }
}

#endif
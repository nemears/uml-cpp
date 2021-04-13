#ifndef ELEMENT_PARSER_H
#define ELEMENT_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/element.h"
#include "uml/parsers/parserMetaData.h"

namespace UML {
    namespace Parsers {

        class CheckOwnedElementFunctor : public AbstractPostProcessFunctor {
            public:
                CheckOwnedElementFunctor(Element* el, YAML::Node node) : AbstractPostProcessFunctor(el, node) {};
                void operator()(Element& el) const override;
        };

        void parseElement(YAML::Node node, Element& el, ParserMetaData& data);

        class ImproperOwnedElementException : public exception {
            private:
                string msg;

            public:
                ImproperOwnedElementException(YAML::Node node, boost::uuids::uuid id) : 
                    msg("Improper owned element parsed, el: " + boost::lexical_cast<string>(id) + " at line " + to_string(node.Mark().line))
                    {};
                virtual const char* what() const throw() {
                    return msg.c_str();
                }
        };
    }
}

#endif
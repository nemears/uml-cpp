#ifndef ELEMENT_PARSER_H
#define ELEMENT_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/element.h"
#include "uml/parsers/parserMetaData.h"

namespace UML {
    namespace Parsers {
        void parseElement(YAML::Node node, Element& el, ParserMetaData& data);
    }
}

#endif
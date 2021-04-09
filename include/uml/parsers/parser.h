#ifndef PARSER_H
#define PARSER_H

#include "packageParser.h"
#include "yaml-cpp/yaml.h"
#include "uml/element.h"

namespace UML {
    namespace Parsers {
        /**
         * The idea behind this function is to have one way to parse any uml config file no matter the parent type.
         * To do this the function will have to read the first node and determine type to assign proper parent parser.
         **/
        Element* parse(YAML::Node node);
    }
}

#endif
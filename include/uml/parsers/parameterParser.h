#ifndef PARAMETER_PARSER_H
#define PARAMETER_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/parameter.h"
#include "uml/parsers/parserMetaData.h"

namespace UML {
    namespace Parsers {
        void parseParameter(YAML::Node node, Parameter& el, ParserMetaData& data);
    }
}

#endif
#ifndef CLASS_PARSER_H
#define CLASS_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/class.h"
#include "uml/parsers/parserMetaData.h"

namespace UML {
    namespace Parsers {
        void parseClass(YAML::Node node, Class& clazz, ParserMetaData& data);
    }
}

#endif
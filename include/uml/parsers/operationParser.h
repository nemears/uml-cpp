#ifndef OPERATION_PARSER_H
#define OPERATION_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/operation.h"
#include "uml/parsers/parserMetaData.h"

namespace UML {
    namespace Parsers {
        void parseOperation(YAML::Node node, Operation& op, ParserMetaData& data);
    }
}

#endif
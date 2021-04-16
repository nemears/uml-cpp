#ifndef OPAQUE_BEHAVIOR_PARSER_H
#define OPAQUE_BEHAVIOR_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/opaqueBehavior.h"
#include "uml/parsers/parserMetaData.h"

namespace UML {
    namespace Parsers {
        void parseOpaqueBehavior(YAML::Node node, OpaqueBehavior& bhv, ParserMetaData& data);
    }
}

#endif
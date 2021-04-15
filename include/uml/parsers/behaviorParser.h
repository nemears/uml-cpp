#ifndef BEHAVIOR_PARSER_H
#define BEHAVIOR_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/behavior.h"
#include "uml/parsers/parserMetaData.h"

namespace UML {
    namespace Parsers {
        void parseBehavior(YAML::Node node, Behavior& bhv, ParserMetaData& data);
    }
}

#endif
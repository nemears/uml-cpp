#ifndef NAMESPACE_PARSER_H
#define NAMESPACE_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/namespace.h"

namespace UML {
    namespace Parsers {
        void parseNamespace(YAML::Node node, Namespace& nmspc);
    }
}

#endif
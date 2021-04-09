#ifndef PACKAGE_PARSER_H
#define PACKAGE_PARSER_H

// #include "namespaceParser.h"
// #include "packageableElementParser.h"
#include "uml/package.h"
#include "yaml-cpp/yaml.h"

namespace UML {
    namespace Parsers {
        void parsePackage(YAML::Node node, Package& pckg);
        void emitPackage(YAML::Emitter& emitter, Package& el);
    }
}

#endif
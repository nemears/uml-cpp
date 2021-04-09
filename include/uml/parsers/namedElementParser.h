#ifndef NAMED_ELEMENT_PARSER_H
#define NAMED_ELEMENT_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/namedElement.h"

namespace UML {
    namespace Parsers {
        void parseNamedElement(YAML::Node node, NamedElement& el);
    }
}

#endif
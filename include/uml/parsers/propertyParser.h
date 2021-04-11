#ifndef PROPERTY_PARSER_H
#define PROPERTY_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/property.h"

namespace UML {
    namespace Parsers {
        void parseProperty(YAML::Node node, Property& prop);
    }
}

#endif
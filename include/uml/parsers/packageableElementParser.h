#ifndef PACKAGEABLE_ELEMENT_PARSER_H
#define PACKAGEABLE_ELEMENT_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/packageableElement.h"

namespace UML {
    namespace Parsers {
        void parsePackageableElement(YAML::Node node, PackageableElement& el);
    }
}

#endif
#ifndef CLASSIFIER_PARSER_H
#define CLASSIFIER_PARSER_H

#include "yaml-cpp/yaml.h"
#include "uml/classifier.h"

namespace UML {
    namespace Parsers {
        void parseClassifier(YAML::Node node, Classifier& clazz);
    }
}

#endif
#include "uml/parsers/classifierParser.h"
#include "uml/parsers/namespaceParser.h"
#include "uml/parsers/propertyParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseClassifier(YAML::Node node, Classifier& clazz) {
    parseNamespace(node, clazz);

    if (node["attributes"]) {
        if (node["attributes"].IsSequence()) {
            for (size_t i=0; i<node["attributes"].size(); i++) {
                if (node["attributes"][i]["property"]) {
                    if (node["attributes"][i]["property"].IsMap()) {
                        Property* prop = new Property;
                        parseProperty(node["attributes"][i]["property"], *prop);
                        clazz.getAttributes().add(*prop);
                    } else {
                        // error
                    }
                }
            }
        } else {
            // error
        }
    }
}
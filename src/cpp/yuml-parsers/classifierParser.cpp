#include "yuml-parsers/classifierParser.h"

using namespace UML;

bool ClassifierParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["attributes"]) {
        if (node["attributes"].IsSequence()) {
            for (std::size_t i=0; i<node["attributes"].size(); i++) {
                if (node["attributes"][i]["property"]) {
                    PropertyParser propertyParser(elements, postProcessFlag);
                    Element* parsedEl = propertyParser.TypedElementParser::parseElement(node["attributes"][i]["property"]);
                    dynamic_cast<Classifier*>(el)->ownedAttributes.push_back(dynamic_cast<Property*>(parsedEl));
                    dynamic_cast<Property*>(parsedEl)->setClassifier(dynamic_cast<Classifier*>(el));
                } else if (node["attributes"][i]["other types here"]) {
                    // TODO TODO TODO
                }
            }
        } else {
            // ERROR
            throw InvalidNodeTypeException(node["attributes"].Mark().line, "sequence");
        }
    }
    return NamedElementParser::parseFeatures(node, el);
}

bool ClassifierParser::emit(YAML::Emitter& emitter, Element* el) {

    bool ret = NamedElementParser::emit(emitter, el);

    if (!dynamic_cast<Classifier*>(el)->ownedAttributes.empty()) {
        emitter << YAML::Key << "attributes";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& property: dynamic_cast<Classifier*>(el)->ownedAttributes) {
            PropertyParser pp(elements, postProcessFlag);
            pp.emit(emitter, property);
        }
        emitter << YAML::EndSeq;
    }

    return ret;
}
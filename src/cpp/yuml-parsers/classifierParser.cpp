#include "yuml-parsers/classifierParser.h"

bool ClassifierParser::parseFeatures(YAML::Node node, UML::Element* el) {
    if (node["attributes"]) {
        if (node["attributes"].IsSequence()) {
            for (std::size_t i=0; i<node["attributes"].size(); i++) {
                if (node["attributes"][i]["property"]) {
                    PropertyParser propertyParser(elements);
                    UML::Element* parsedEl = propertyParser.parseElement(node["attributes"][i]["property"]);
                    ((UML::Classifier*) el)->ownedAttributes.push_back((UML::Property*) parsedEl);
                } else if (node["attributes"][i]["other types here"]) {
                    // TODO TODO TODO
                }
            }
        } else {
            // ERROR
        }
    }
    return NamedElementParser::parseFeatures(node, el);
}

bool ClassifierParser::emit(YAML::Emitter& emitter, Element* el) {
    if (!((Classifier*)el)->ownedAttributes.empty()) {
        emitter << YAML::Key << "attributes";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& property: ((Classifier*)el)->ownedAttributes) {
            PropertyParser pp(elements);
            pp.emit(emitter, el);
        }
        emitter << YAML::EndSeq;
    }

    return NamedElementParser::emit(emitter, el);
}
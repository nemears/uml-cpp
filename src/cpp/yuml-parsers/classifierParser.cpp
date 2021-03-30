#include "yuml-parsers/classifierParser.h"

using namespace UML;

bool ClassifierParser::parseFeatures(YAML::Node node, Element* el) {

    bool ret = NamedElementParser::parseFeatures(node, el);

    if (node["attributes"]) {
        if (node["attributes"].IsSequence()) {
            for (std::size_t i=0; i<node["attributes"].size(); i++) {
                if (node["attributes"][i]["property"]) {
                    PropertyParser propertyParser(elements, postProcessFlag);
                    Element* parsedEl = propertyParser.TypedElementParser::parseElement(node["attributes"][i]["property"]);
                    dynamic_cast<Classifier*>(el)->getAttributes().add(*dynamic_cast<Property*>(parsedEl));
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

    if (node["generalizations"]) {
        if (node["generalizations"].IsSequence()) {
            for (size_t i = 0; i<node["generalizations"].size(); i++) {
                if (node["generalizations"][i].IsScalar()) {
                    // TODO this shouldn't be valid, generalizations should be specified here only from specific so it should never be scalar
                    // Not removing yet because maybe there is some edge case where it will be defined elsewhere but not sure what that would be
                    if (isValidUUID4(node["generalizations"][i].as<string>())) {
                        parseNowOrLater(boost::lexical_cast<boost::uuids::uuid>(node["generalizations"][i].as<string>()),
                                        el->getID(),
                                        node["generalizations"][i],
                                        &ClassifierParser::addGeneralizationLater);
                    }
                }else if (node["generalizations"][i]["generalization"]) {
                    if (node["generalizations"][i]["generalization"].IsMap()) {
                        GeneralizationParser gp(elements, postProcessFlag);
                        Element* parsedEl = gp.parseElement(node["generalizations"][i]["generalization"]);
                    }
                }
            }
        }
    }
    
    return ret;
}

void ClassifierParser::addGeneralizationLater(YAML::Node node, Element* classifier, Element* generalization) {
    dynamic_cast<Classifier*>(classifier)->getGeneralizations().add(*dynamic_cast<Generalization*>(generalization));
}

bool ClassifierParser::emit(YAML::Emitter& emitter, Element* el) {

    bool ret = NamedElementParser::emit(emitter, el);

    if (!dynamic_cast<Classifier*>(el)->getAttributes().empty()) {
        emitter << YAML::Key << "attributes";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& property: dynamic_cast<Classifier*>(el)->getAttributes()) {
            PropertyParser pp(elements, postProcessFlag);
            pp.emit(emitter, property);
        }
        emitter << YAML::EndSeq;
    }

    return ret;
}
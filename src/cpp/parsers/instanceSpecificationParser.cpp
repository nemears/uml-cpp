#include "headers/instanceSpecificationParser.h"
#include "../uml/headers/instanceSpecification.h"

Element* InstanceSpecificationParser::createElement() {
    return new InstanceSpecification;
}

bool InstanceSpecificationParser::parseFeatures(YAML::Node node, UML::Element* el) {

    if(node["classifier"]) {
        string parsedId = node["classifier"].as<string>();
        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid classifierId = boost::lexical_cast<boost::uuids::uuid>(parsedId);
            Classifier* instClassifier = (Classifier*) (*elements)[classifierId];
            ((InstanceSpecification*) el)->setClassifier(instClassifier);
        } else {
            // TODO error
        }
    }

    if (node["slots"]) {
        for (std::size_t i = 0; i < node["slots"].size(); i++) {
            // TODO slot parser
        }
    }

    return ClassifierParser::parseFeatures(node, el);
}
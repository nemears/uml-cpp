#include "yuml-parsers/instanceSpecificationParser.h"
#include "uml/instanceSpecification.h"
#include "yuml-parsers/slotParser.h"

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
            if (node["slots"][i]["slot"]) {
                SlotParser slotParser(elements);
                Slot* parsedEl = (Slot*) slotParser.parseElement(node["slots"][i]["slot"]);
                ((InstanceSpecification*) el)->slots.push_back(parsedEl);
            }
        }
    }

    return ClassifierParser::parseFeatures(node, el);
}

bool InstanceSpecificationParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::INSTANCE_SPECIFICATION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "instanceSpecification";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = ClassifierParser::emit(emitter, el);

    if (((InstanceSpecification*)el)->getClassifier() != NULL) {
        emitter << YAML::Key << "classifier";
        emitter << YAML::Value << boost::lexical_cast<string>(((InstanceSpecification*)el)->getClassifier()->uuid);
    }

    if (!((InstanceSpecification*)el)->slots.empty()) {
        emitter << YAML::Key << "slots";
        emitter << YAML::Value << YAML::BeginSeq;

        for (auto const& slot: ((InstanceSpecification*)el)->slots) {
            // TODO
        }
    }

    if (el->getElementType() == ElementType::INSTANCE_SPECIFICATION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}
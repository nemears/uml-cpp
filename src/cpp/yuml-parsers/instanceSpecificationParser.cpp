#include "yuml-parsers/instanceSpecificationParser.h"
#include "uml/instanceSpecification.h"
#include "yuml-parsers/slotParser.h"

Element* InstanceSpecificationParser::createElement() {
    return new InstanceSpecification;
}

bool InstanceSpecificationParser::parseFeatures(YAML::Node node, Element* el) {

    if(node["classifier"]) {
        string parsedId = node["classifier"].as<string>();
        if (UML::isValidUUID4(parsedId)) {
            boost::uuids::uuid classifierId = boost::lexical_cast<boost::uuids::uuid>(parsedId);
            Classifier* instClassifier = dynamic_cast<Classifier*>((*elements)[classifierId]);
            dynamic_cast<InstanceSpecification*>(el)->setClassifier(instClassifier);
        } else {
            // error
            throw el->invalidID_Exception;
        }
    }

    if (node["slots"]) {
        for (std::size_t i = 0; i < node["slots"].size(); i++) {
            if (node["slots"][i]["slot"]) {
                SlotParser slotParser(elements);
                Slot* parsedEl = (Slot*) slotParser.parseElement(node["slots"][i]["slot"]);
                dynamic_cast<InstanceSpecification*>(el)->slots.push_back(parsedEl);
            }
        }
    }

    return NamedElementParser::parseFeatures(node, el);
}

bool InstanceSpecificationParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::INSTANCE_SPECIFICATION) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "instanceSpecification";
        emitter << YAML::Value << YAML::BeginMap;
    }

    bool ret = NamedElementParser::emit(emitter, el);

    if (dynamic_cast<InstanceSpecification*>(el)->getClassifier() != NULL) {
        emitter << YAML::Key << "classifier";
        emitter << YAML::Value << boost::lexical_cast<string>(dynamic_cast<InstanceSpecification*>(el)->getClassifier()->uuid);
    }

    if (!dynamic_cast<InstanceSpecification*>(el)->slots.empty()) {
        emitter << YAML::Key << "slots";
        emitter << YAML::Value << YAML::BeginSeq;

        for (auto const& slot: dynamic_cast<InstanceSpecification*>(el)->slots) {
            SlotParser sp(elements);
            if(!sp.emit(emitter, slot)) {
                return false;
            }
        }

        emitter << YAML::EndSeq;
    }

    if (el->getElementType() == ElementType::INSTANCE_SPECIFICATION) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return ret;
}
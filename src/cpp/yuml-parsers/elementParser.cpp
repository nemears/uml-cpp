#include "yuml-parsers/elementParser.h"
#include "yuml-parsers/classParser.h"
#include "yuml-parsers/instanceSpecificationParser.h"

bool ElementParser::parseFeatures(YAML::Node node, UML::Element* el) {
    if (node["id"]) {
        boost::uuids::uuid oldId = el->uuid;
        try {
            el->setID(node["id"].as<string>());

            // override elements entry
            elements->erase(oldId);
            (*elements)[el->uuid] = el;
        } catch (exception& e) {
            cerr << e.what() << '\n';
        }
    } 
    if (node["children"]) {
        // TODO
        if (node["children"].IsSequence()) {
            for (std::size_t i=0; i<node["children"].size(); i++) {
                if (node["children"][i]["class"]) {
                    ClassParser classParser(elements);
                    UML::Element* parsedEl = classParser.parseElement(node["children"][i]["class"]);
                    el->ownedElements.push_back(parsedEl);
                } else if (node["children"][i]["instanceSpecification"]){
                    InstanceSpecificationParser instanceParser(elements);
                    UML::Element* parsedEl = instanceParser.parseElement(node["children"][i]["instanceSpecification"]);
                    el->ownedElements.push_back(parsedEl);
                } else if (node["children"][i]["other types here"]) {
                    // TODO TODO TODO
                }
            }
        } else {
            // ERROR
        }
    }
    return true;
}

bool ElementParser::emit(YAML::Emitter& emitter, Element* el) {

    emitter << YAML::BeginMap;
    emitter << YAML::Key << "id";
    emitter << YAML::Value << boost::lexical_cast<string>(el->uuid);
    emitter << YAML::EndMap;

    if (!el->ownedElements.empty()){
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "ownedElements";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& child: el->ownedElements) {
            // emit children, problem is we need to know what type they are so we can choose the right parser
        }
        emitter << YAML::EndSeq;
    }

    return true;
}
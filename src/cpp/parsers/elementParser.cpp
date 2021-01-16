#include "headers/elementParser.h"
#include "headers/classParser.h"
#include "headers/instanceSpecificationParser.h"

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
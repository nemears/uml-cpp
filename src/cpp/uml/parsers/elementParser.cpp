#include "uml/parsers/elementParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseElement(YAML::Node node, Element& el, ParserMetaData& data) {
    if (node["id"]) {
        if (node["id"].IsScalar()) {
            string id = node["id"].as<string>();
            if (isValidUUID4(id)) {
                el.setID(boost::lexical_cast<boost::uuids::uuid>(id));
            } else {
                // error
            }
        } else {
            // error
        }
    }

    data.elements.add(el);

    // TODO change tests
    if (node["ownedElements"]) {
        // TODO validate
    }
}
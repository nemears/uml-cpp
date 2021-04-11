#include "uml/parsers/typedElementParser.h"
#include "uml/parsers/namedElementParser.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parseTypedElement(YAML::Node node, TypedElement& el, ParserMetaData& data) {

    parseNamedElement(node, el, data);

    if (node["type"]) {
        if (node["type"].IsScalar()) {
            string typeIDstring = node["type"].as<string>();
            if (isValidUUID4(typeIDstring)) {
                boost::uuids::uuid typeID = boost::lexical_cast<boost::uuids::uuid>(typeIDstring);
                if (data.elements.count(typeID)) {
                    if (data.elements.get(typeID)->isSubClassOf(ElementType::TYPE)) {
                        el.setType(dynamic_cast<Type*>(data.elements.get(typeID)));
                    } else {
                        // error
                    }
                } else {
                    // TODO add flag to set later in metadata
                }
            } else {
                // error
            }
        } else {
            // error
        }
    }
}
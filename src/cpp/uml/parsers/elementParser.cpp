#include "uml/parsers/elementParser.h"
#include <iostream>

using namespace UML;
using namespace Parsers;

void UML::Parsers::CheckOwnedElementFunctor::operator()(Element& el) const {
    if (!m_el->getOwnedElements().count(el.getID())) {
        throw ImproperOwnedElementException(m_node, el.getID());
    }
}

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
        if (node["ownedElements"].IsSequence()) {
            for (size_t i=0; i<node["ownedElements"].size(); i++) {
                if (node["ownedElements"][i].IsScalar()) {
                    string id = node["ownedElements"][i].as<string>();
                    if (isValidUUID4(id)) {
                        applyFunctor(data, boost::lexical_cast<boost::uuids::uuid>(id), new CheckOwnedElementFunctor(&el, node["ownedElements"][i]));
                    } else {
                        // error
                    }
                } else {
                    // error
                }
            }
        } else {
            // error
        }
    }
}
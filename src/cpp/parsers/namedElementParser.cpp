#include "headers/namedElementParser.h"

bool NamedElementParser::parseFeatures(YAML::Node node, UML::Element* el) {

    if (node["name"]) {
        if (((UML::NamedElement*)el)->getName().compare("") != 0) {
            cout << "[Error] " << keyword << " " << ((UML::NamedElement*)el)->getName() << " duplicate name found";
            cout << " line number " << node["name"].Mark().line << '\n';
        }

        try {
            ((UML::NamedElement*)el)->setName(node["name"].as<string>());
            cout << "[Info] " << keyword << " name: " << ((UML::NamedElement*)el)->getName() << "\n";
        } catch(exception& e) {
                cerr << e.what() << '\n';
        }

    }

    return ElementParser::parseFeatures(node, el);
}
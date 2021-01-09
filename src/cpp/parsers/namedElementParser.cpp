#include "headers/namedElementParser.h"

bool NamedElementParser::parseTag(InputParser* input, Element* el) {
    if (input->firstWord.compare("name") == 0) {

        // Check if we are overiding
        if (((NamedElement*)el)->getName().compare("") != 0) {
            cout << "[Error] " << keyword << " " << ((NamedElement*)el)->getName() << " duplicate name found";
            cout << " line number " << input->lineNumber << '\n';
            return false;
        }

        try {
            ((NamedElement*)el)->setName(input->getTag());
            cout << "[Info] " << keyword << " name: " << ((NamedElement*)el)->getName() << "\n";
            return true;
        } catch(exception& e) {
                cerr << e.what() << '\n';
        }
        return true;
    }

    return ElementParser::parseTag(input, el);
}

bool NamedElementParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["name"]) {
        if (((NamedElement*)el)->getName().compare("") != 0) {
            cout << "[Error] " << keyword << " " << ((NamedElement*)el)->getName() << " duplicate name found";
            cout << " line number " << node["name"].Mark().line << '\n';
        }

        try {
            ((NamedElement*)el)->setName(node["name"].as<string>());
            cout << "[Info] " << keyword << " name: " << ((NamedElement*)el)->getName() << "\n";
        } catch(exception& e) {
                cerr << e.what() << '\n';
        }

    }

    return ElementParser::parseFeatures(node, el);
}
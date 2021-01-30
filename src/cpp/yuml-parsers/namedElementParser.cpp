#include "yuml-parsers/namedElementParser.h"

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

bool NamedElementParser::emit(YAML::Emitter& emitter, Element* el) {

    if (((NamedElement*) el)->getName().compare("") != 0) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "name";
        emitter << YAML::Value << ((NamedElement*) el)->getName();
    }

    return ElementParser::emit(emitter, el);

}
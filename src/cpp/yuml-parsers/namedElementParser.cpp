#include "yuml-parsers/namedElementParser.h"

bool NamedElementParser::parseFeatures(YAML::Node node, Element* el) {

    if (node["name"]) {
        if (dynamic_cast<NamedElement*>(el)->getName().compare("") != 0) {
            cout << "[Error] " << keyword << " " << dynamic_cast<NamedElement*>(el)->getName() << " duplicate name found";
            cout << " line number " << node["name"].Mark().line << '\n';
        }

        try {
            dynamic_cast<NamedElement*>(el)->setName(node["name"].as<string>());
            cout << "[Info] " << keyword << " name: " << dynamic_cast<NamedElement*>(el)->getName() << "\n";
        } catch(exception& e) {
                cerr << e.what() << '\n';
        }

    }

    return ElementParser::parseFeatures(node, el);
}

bool NamedElementParser::emit(YAML::Emitter& emitter, Element* el) {

    if (dynamic_cast<NamedElement*>(el)->getName().compare("") != 0) {
        emitter << YAML::Key << "name";
        emitter << YAML::Value << dynamic_cast<NamedElement*>(el)->getName();
    }

    return ElementParser::emit(emitter, el);

}
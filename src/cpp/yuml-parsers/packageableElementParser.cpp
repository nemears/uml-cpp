#include "yuml-parsers/packageableElementParser.h"

bool PackageableElementParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamedElementParser::parseFeatures(node, el);

    parsePackageableElementFeatures(node, el);

    return ret;
}

bool PackageableElementParser::parsePackageableElementFeatures(YAML::Node node, Element* el) {
    if (node["owningPackage"]) {
        if (node["owningPackage"].IsScalar()) {
            string owningPackageID = node["owningPackage"].as<string>();
            if (isValidUUID4(owningPackageID)) {
                // Validate this now or later
            } else {
                // ERROR
            }
        } else {
            // ERROR
        }
    }

    return true;
}

bool PackageableElementParser::emit(YAML::Emitter& emitter, Element* el) {
    bool ret = NamedElementParser::emit(emitter, el);

    return ret;
}
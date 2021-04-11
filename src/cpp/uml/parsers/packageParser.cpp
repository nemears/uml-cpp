#include "uml/parsers/packageParser.h"
#include "uml/parsers/packageableElementParser.h"
#include "uml/parsers/namespaceParser.h"
#include "uml/activity.h"

using namespace UML;
using namespace Parsers;

void UML::Parsers::parsePackage(YAML::Node node, Package& pckg, ParserMetaData& data) {

    parseNamespace(node, pckg, data);

    parsePackageableElement(node, pckg, data);

    if (node["packagedElements"]) {
        if (node["packagedElements"].IsSequence()) {
            for (size_t i=0; i<node["packagedElements"].size(); i++) {
                if (node["packagedElements"][i].IsMap()) {
                    if (node["packagedElements"][i]["activity"]) {
                        Activity* activity = new Activity;
                        // TODO parse activity
                        activity->setOwningPackage(&pckg);

                    // TODO all the others

                    } else if (node["packagedElements"][i]["package"]) {
                        Package* package = new Package;
                        UML::Parsers::parsePackage(node["packagedElements"][i]["package"], *package, data);
                        package->setOwningPackage(&pckg);
                    }
                } else {
                    //error
                }
            }
        } else {
            // error
        }
    }
}
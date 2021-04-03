#include "yuml-parsers/packageParser.h"
#include "yuml-parsers/classParser.h"
#include "yuml-parsers/instanceSpecificationParser.h"
#include "yuml-parsers/opaqueBehaviorParser.h"
#include "yuml-parsers/parameterParser.h"
#include "yuml-parsers/namespaceParser.h"
#include "yuml-parsers/activityParser.h"

bool PackageParser::parseFeatures(YAML::Node node, Element* el) {
    bool ret = NamespaceParser::parseFeatures(node, el);

    if (ret) {
        ret = parsePackageableElementFeatures(node, el);
    }

    if (node["packagedElements"]) {
        if (node["packagedElements"].IsSequence()) {
            for (size_t i=0; i<node["packagedElements"].size(); i++) {
                if (node["packagedElements"][i].IsMap()) {
                    if (node["packagedElements"][i]["activity"]) {
                        ActivityParser activityParser(NamespaceParser::elements, NamespaceParser::postProcessFlag);
                        Element* parsedEl = activityParser.parseElement(node["packagedElements"][i]["activity"]);
                        dynamic_cast<Package*>(el)->getPackagedElements().add(*dynamic_cast<PackageableElement*>(parsedEl));
                    } else if (node["packagedElements"][i]["class"]) {
                        ClassParser classParser(NamespaceParser::elements, NamespaceParser::postProcessFlag);
                        Element* parsedEl = classParser.parseElement(node["packagedElements"][i]["class"]);
                        dynamic_cast<Package*>(el)->getPackagedElements().add(*dynamic_cast<PackageableElement*>(parsedEl));
                    } else if (node["packagedElements"][i]["instanceSpecification"]){
                        InstanceSpecificationParser instanceParser(NamespaceParser::elements, NamespaceParser::postProcessFlag);
                        Element* parsedEl = instanceParser.parseElement(node["packagedElements"][i]["instanceSpecification"]);
                        dynamic_cast<Package*>(el)->getPackagedElements().add(*dynamic_cast<PackageableElement*>(parsedEl));
                    } else if (node["packagedElements"][i]["namespace"]) {
                        NamespaceParser namespaceParser(NamespaceParser::elements, NamespaceParser::postProcessFlag);
                        Element* parsedEl = namespaceParser.parseElement(node["packagedElements"][i]["namespace"]);
                        dynamic_cast<Package*>(el)->getPackagedElements().add(*dynamic_cast<PackageableElement*>(parsedEl));
                    } else if (node["packagedElements"][i]["opaqueBehavior"]) {
                        OpaqueBehaviorParser opaqueBehaviorParser(NamespaceParser::elements, NamespaceParser::postProcessFlag);
                        Element* parsedEl = opaqueBehaviorParser.parseElement(node["packagedElements"][i]["opaqueBehavior"]);
                        el->getOwnedElements().add(*parsedEl);
                    } else if (node["packagedElements"][i]["package"]) {
                        PackageParser packageParser(NamespaceParser::elements, NamespaceParser::postProcessFlag);
                        Element* parsedEl = packageParser.NamespaceParser::parseElement(node["packagedElements"][i]["package"]);
                        el->getOwnedElements().add(*parsedEl);
                    } else {
                        // ERROR, string could not be identified to uml schema
                        throw InvalidIdentifierException(node["packagedElements"][i].Mark().line, node["packagedElements"][i].Scalar());
                    }
                } else {
                    // error or defined elsewhere
                }
            }
        } else {
            // Error
        }
    }

    return ret;
}

Element* PackageParser::createElement() {
    NamespaceParser::theEl = new Package;
    return NamespaceParser::theEl;
}

bool PackageParser::emit(YAML::Emitter& emitter, Element* el) {
    if (el->getElementType() == ElementType::PACKAGE) {
        emitter << YAML::BeginMap;
        emitter << YAML::Key << "package";
        emitter << YAML::Value << YAML::BeginMap;
    }

    if (el->getElementType() == ElementType::PACKAGE) {
        emitter << YAML::EndMap;
        emitter << YAML::EndMap;
    }

    return true;
}
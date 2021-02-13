#include "yuml-parsers/elementParser.h"
#include "yuml-parsers/classParser.h"
#include "yuml-parsers/instanceSpecificationParser.h"
#include "yuml-parsers/opaqueBehaviorParser.h"
#include "yuml-parsers/parameterParser.h"
#include "yuml-parsers/namespaceParser.h"
#include "yuml-parsers/activityParser.h"

using namespace UML;

bool ElementParser::parseFeatures(YAML::Node node, Element* el) {
    if (node["id"]) {
        boost::uuids::uuid oldId = el->uuid;
        try {
            el->setID(node["id"].as<string>());

            // override elements entry
            elements->erase(oldId);
            (*elements)[el->uuid] = el;
        } catch (exception& e) {
            cerr << e.what() << '\n';
        }
    } 

    if (node["children"]) {
        // TODO
        if (node["children"].IsSequence()) {
            for (std::size_t i=0; i<node["children"].size(); i++) {
                if (node["children"][i]["activity"]) {
                    ActivityParser activityParser(elements);
                    Element* parsedEl = activityParser.parseElement(node["children"][i]["activity"]);
                    el->ownedElements.push_back(parsedEl);
                } else if (node["children"][i]["class"]) {
                    ClassParser classParser(elements);
                    Element* parsedEl = classParser.parseElement(node["children"][i]["class"]);
                    el->ownedElements.push_back(parsedEl);
                } else if (node["children"][i]["instanceSpecification"]){
                    InstanceSpecificationParser instanceParser(elements);
                    Element* parsedEl = instanceParser.parseElement(node["children"][i]["instanceSpecification"]);
                    el->ownedElements.push_back(parsedEl);
                } else if (node["children"][i]["namespace"]) {
                    NamespaceParser namespaceParser(elements);
                    Element* parsedEl = namespaceParser.parseElement(node["children"][i]["namespace"]);
                    el->ownedElements.push_back(parsedEl);
                }else if (node["children"][i]["opaqueBehavior"]) {
                    OpaqueBehaviorParser opaqueBehaviorParser(elements);
                    Element* parsedEl = opaqueBehaviorParser.parseElement(node["children"][i]["opaqueBehavior"]);
                    el->ownedElements.push_back(parsedEl);
                } else if (node["children"][i]["parameter"]) {
                    ParameterParser parameterParser(elements);
                    Element* parsedEl = parameterParser.parseElement(node["children"][i]["parameter"]);
                    el->ownedElements.push_back(parsedEl);
                } 
                
                // TODO literals?
                
                else {
                    // ERROR, string could not be identified to uml schema
                    throw InvalidIdentifierException(node["children"][i].Mark().line, node["children"][i].Scalar());
                }
            }
        } else {
            // ERROR, was not a sequence
            throw InvalidNodeTypeException(node["children"].Mark().line, "sequence");
        }
    }
    
    return true;
}

bool ElementParser::emit(YAML::Emitter& emitter, Element* el) {

    emitter << YAML::Key << "id";
    emitter << YAML::Value << boost::lexical_cast<string>(el->uuid);

    if (!el->ownedElements.empty()){
        emitter << YAML::Key << "children";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& child: el->ownedElements) {
            switch (child->getElementType()) {
                case ElementType::CLASS : {
                    ClassParser cp(elements);
                    if (!cp.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::INSTANCE_SPECIFICATION : {
                    InstanceSpecificationParser ip(elements);
                    if (!ip.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::NAMESPACE : {
                    NamespaceParser np(elements);
                    if (!np.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::OPAQUE_BEHAVIOR : {
                    OpaqueBehaviorParser obp(elements);
                    if (!obp.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::PARAMETER : {
                    ParameterParser pp(elements);
                    if (!pp.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                default : {
                    // Error
                    throw AbstractTypeEmitException(child->getElementTypeString(), boost::lexical_cast<string>(child->uuid));
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    return true;
}
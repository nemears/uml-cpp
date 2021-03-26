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
        boost::uuids::uuid oldId = el->getID();
        try {
            el->setID(node["id"].as<string>());

            // override elements entry
            elements->erase(oldId);
            (*elements)[el->getID()] = el;
        } catch (exception& e) {
            cerr << e.what() << '\n';
        }
    } 

    if (node["children"]) {
        // TODO
        if (node["children"].IsSequence()) {
            for (std::size_t i=0; i<node["children"].size(); i++) {
                if (node["children"][i]["activity"]) {
                    ActivityParser activityParser(elements, postProcessFlag);
                    Element* parsedEl = activityParser.parseElement(node["children"][i]["activity"]);
                    el->getOwnedElements().add(*parsedEl);
                } else if (node["children"][i]["class"]) {
                    ClassParser classParser(elements, postProcessFlag);
                    Element* parsedEl = classParser.parseElement(node["children"][i]["class"]);
                    el->getOwnedElements().add(*parsedEl);
                } else if (node["children"][i]["instanceSpecification"]){
                    InstanceSpecificationParser instanceParser(elements, postProcessFlag);
                    Element* parsedEl = instanceParser.parseElement(node["children"][i]["instanceSpecification"]);
                    el->getOwnedElements().add(*parsedEl);
                } else if (node["children"][i]["namespace"]) {
                    NamespaceParser namespaceParser(elements, postProcessFlag);
                    Element* parsedEl = namespaceParser.parseElement(node["children"][i]["namespace"]);
                    el->getOwnedElements().add(*parsedEl);
                }else if (node["children"][i]["opaqueBehavior"]) {
                    OpaqueBehaviorParser opaqueBehaviorParser(elements, postProcessFlag);
                    Element* parsedEl = opaqueBehaviorParser.parseElement(node["children"][i]["opaqueBehavior"]);
                    el->getOwnedElements().add(*parsedEl);
                } else if (node["children"][i]["parameter"]) {
                    ParameterParser parameterParser(elements, postProcessFlag);
                    Element* parsedEl = parameterParser.TypedElementParser::parseElement(node["children"][i]["parameter"]);
                    el->getOwnedElements().add(*parsedEl);
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
    emitter << YAML::Value << boost::lexical_cast<string>(el->getID());

    if (!el->getOwnedElements().empty()){
        emitter << YAML::Key << "children";
        emitter << YAML::Value << YAML::BeginSeq;
        for (auto const& child: el->getOwnedElements()) {
            switch (child->getElementType()) {
                case ElementType::ACTIVITY : {
                    ActivityParser ap(elements, postProcessFlag);
                    if(!ap.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::CLASS : {
                    ClassParser cp(elements, postProcessFlag);
                    if (!cp.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::INSTANCE_SPECIFICATION : {
                    InstanceSpecificationParser ip(elements, postProcessFlag);
                    if (!ip.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::NAMESPACE : {
                    NamespaceParser np(elements, postProcessFlag);
                    if (!np.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::OPAQUE_BEHAVIOR : {
                    OpaqueBehaviorParser obp(elements, postProcessFlag);
                    if (!obp.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::PARAMETER : {
                    ParameterParser pp(elements, postProcessFlag);
                    if (!pp.emit(emitter, child)) {
                        return false;
                    }
                    break;
                }
                case ElementType::PROPERTY : {
                    // if appropriate check and skip
                    if (el->isSubClassOf(ElementType::CLASSIFIER)) {
                        if (dynamic_cast<Classifier*>(el)->getAttributes().count(child->getID())) {
                            emitter << YAML::Value << child->getIDstring();
                            break;
                        }
                    }
                }
                default : {
                    // Error
                    throw AbstractTypeEmitException(child->getElementTypeString(), boost::lexical_cast<string>(child->getID()));
                }
            }
        }
        emitter << YAML::EndSeq;
    }

    return true;
}
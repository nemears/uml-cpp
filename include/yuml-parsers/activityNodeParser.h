#ifndef ACTIVITYNODEPARSER
#define ACTIVITYNODEPARSER

#include "namedElementParser.h"
#include "uml/activityNode.h"
#include "controlFlowParser.h"
#include "objectFlowParser.h"

class ActivityNodeParser : virtual public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
        bool parseActivityNodeFeatures(YAML::Node node, Element* el);
        bool emitActivityNode(YAML::Emitter& emitter, Element* el);
    public:
        ActivityNodeParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamedElementParser(elements, postParsers){};

        bool emit(YAML::Emitter& emitter, Element* el) override;
        static void addIncomingEdgeLater(YAML::Node node, Element* activityNode, Element* incomingEdge) {
            dynamic_cast<ActivityNode*>(activityNode)->getIncoming().add(*dynamic_cast<ActivityEdge*>(incomingEdge));
        }
        static void addOutgoingEdgeLater(YAML::Node node, Element* activityNode, Element* outgoingEdge) {
            dynamic_cast<ActivityNode*>(activityNode)->getOutgoing().add(*dynamic_cast<ActivityEdge*>(outgoingEdge));
        }
};

#endif
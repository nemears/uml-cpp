#ifndef DECISIONNODEPARSER_H
#define DECISIONNODEPARSER_H

#include "activityNodeParser.h"
#include "uml/decisionNode.h"

class DecisionNodeParser : public ActivityNodeParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        Element* createElement() override;
        DecisionNodeParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) :
            ActivityNodeParser(elements, postParsers),
            NamedElementParser(elements, postParsers)
            {
                keyword = "decisionNode";
            };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static void parseDecisionInputFlowLater(YAML::Node node, Element* decisionNode, Element* decisionInputFlow) {
            // if placeholder delete it
            delete dynamic_cast<DecisionNode*>(decisionNode)->getDecisionInputFlow();

            // set value
            dynamic_cast<DecisionNode*>(decisionNode)->setDecisionInputFlow(dynamic_cast<ObjectFlow*>(decisionInputFlow));
        }
        static DecisionNodeParser createNewParser();
};

#endif
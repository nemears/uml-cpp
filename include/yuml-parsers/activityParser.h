#ifndef ACTIVITYPARSER
#define ACTIVITYPARSER

#include "behaviorParser.h" 
#include "actionParser.h"
#include "objectNodeParser.h"
#include "inputPinParser.h"
#include "outputPinParser.h"
#include "initialNodeParser.h"
#include "finalNodeParser.h"
#include "decisionNodeParser.h"
#include "joinNodeParser.h"
#include "forkNodeParser.h"
#include "mergeNodeParser.h"
#include "parameterNodeParser.h"
#include "controlFlowParser.h"
#include "objectFlowParser.h"
#include "uml/activity.h"

class ActivityParser : public BehaviorParser {
    protected:
        Element* createElement();
        bool parseFeatures(YAML::Node node, Element* el);
    public:
        ActivityParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            BehaviorParser(elements, postParsers) {
            keyword = "activity";
        };
        bool emit(YAML::Emitter& emitter, Element* el) override;
        static ActivityParser createNewParser();
};

#endif
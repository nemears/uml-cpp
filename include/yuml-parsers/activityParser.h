#ifndef ACTIVITYPARSER
#define ACTIVITYPARSER

#include "classParser.h" 
#include "uml/activity.h"

class ActivityParser : public ClassParser {
    protected:
        Element* createElement();
        bool parseFeatures(YAML::Node node, Element* el);
    public:
        ActivityParser(map<boost::uuids::uuid, Element*>* elements) : ClassParser(elements) {
            keyword = "Activity";
        }
        bool emit(YAML::Emitter& emitter, Element* el) override;
};

#endif
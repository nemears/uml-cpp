#ifndef ACTIVITYNODEPARSER
#define ACTIVITYNODEPARSER

#include "namedElementParser.h"
#include "uml/activityNode.h"

class ActivityNodeParser : public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
    public:
        ActivityNodeParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamedElementParser(elements){
            
        };

        bool emit(YAML::Emitter& emitter, Element* el) override;
};

#endif
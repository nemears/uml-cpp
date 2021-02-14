#ifndef ACTIVITYNODEPARSER
#define ACTIVITYNODEPARSER

#include "namedElementParser.h"
#include "uml/activityNode.h"

class ActivityNodeParser : virtual public NamedElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
        bool parseActivityNodeFeatures(YAML::Node node, Element* el);
        bool emitActivityNode(YAML::Emitter& emitter, Element* el);
    public:
        ActivityNodeParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamedElementParser(elements){
            
        };

        bool emit(YAML::Emitter& emitter, Element* el) override;
};

#endif
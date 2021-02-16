#ifndef INSTANCESPECIFICATIONPARSER
#define INSTANCESPECIFICATIONPARSER
#include "namedElementParser.h"

using namespace std;
using namespace UML;

class InstanceSpecificationParser : public NamedElementParser {
    protected:
        Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        InstanceSpecificationParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            NamedElementParser(elements, postParsers){
                keyword = "instanceSpecification";
        };

        bool emit(YAML::Emitter& emitter, Element* el);
        static InstanceSpecificationParser createNewParser();
};

#endif
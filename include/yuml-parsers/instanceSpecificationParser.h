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
        InstanceSpecificationParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamedElementParser(elements){

        };

        bool emit(YAML::Emitter& emitter, Element* el);
};

#endif
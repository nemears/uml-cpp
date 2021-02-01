#ifndef INSTANCESPECIFICATIONPARSER
#define INSTANCESPECIFICATIONPARSER
#include "classifierParser.h"

using namespace std;
using namespace UML;

class InstanceSpecificationParser : public ClassifierParser {
    protected:
        Element* createElement();
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        InstanceSpecificationParser(map<boost::uuids::uuid, UML::Element*>* elements) : ClassifierParser(elements){

        };
        bool emit(YAML::Emitter& emitter, Element* el);
};

#endif
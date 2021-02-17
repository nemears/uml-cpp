#ifndef INSTANCESPECIFICATIONPARSER
#define INSTANCESPECIFICATIONPARSER
#include "namedElementParser.h"
#include "uml/instanceSpecification.h"
#include "yuml-parsers/slotParser.h"

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
        static void setClassifierLater(Element* inst, Element* classifier) {
            dynamic_cast<InstanceSpecification*>(inst)->setClassifier(dynamic_cast<Classifier*>(classifier));
        }
};

#endif
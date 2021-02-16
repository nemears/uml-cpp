#ifndef NAMEDELEMENTPARSER
#define NAMEDELEMENTPARSER
#include "uml/namedElement.h"
#include "elementParser.h"
using namespace std;

class NamedElementParser : public ElementParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
    public:
        NamedElementParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ElementParser(elements, postParsers){
            
        };

        bool emit(YAML::Emitter& emitter, Element* el) override;
};

#endif
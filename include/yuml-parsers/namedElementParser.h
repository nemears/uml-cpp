#ifndef NAMEDELEMENTPARSER
#define NAMEDELEMENTPARSER
#include "uml/namedElement.h"
#include "elementParser.h"
using namespace std;

class NamedElementParser : public ElementParser {
    protected:
        virtual bool parseFeatures(YAML::Node node, UML::Element* el);
    public:
        NamedElementParser(map<boost::uuids::uuid, UML::Element*>* elements) : ElementParser(elements){
            
        };

        bool emit(YAML::Emitter& emitter, Element* el);
};

#endif
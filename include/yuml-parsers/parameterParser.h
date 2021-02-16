#ifndef PARAMETERPARSER
#define PARAMETERPARSER

#include "typedElementParser.h"
#include "multiplicityElementParser.h"
#include "uml/parameter.h"

using namespace std;
using namespace UML;

class ParameterParser : public TypedElementParser, public MultiplicityElementParser {
    protected:
        Element* createElement();
        bool parseFeatures(YAML::Node node, Element* el);

    public:
        ParameterParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            TypedElementParser(elements, postParsers), 
            MultiplicityElementParser(elements, postParsers), 
            NamedElementParser(elements, postParsers), 
            ElementParser(elements, postParsers) {
            this->TypedElementParser::keyword = "parameter";
        }
        bool emit(YAML::Emitter& emitter, Element* el);
        static ParameterParser createNewParser();
};

#endif
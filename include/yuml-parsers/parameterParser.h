#ifndef PARAMETERPARSER
#define PARAMETERPARSER

#include "typedElementParser.h"
#include "uml/parameter.h"

using namespace std;
using namespace UML;

class ParameterParser : public TypedElementParser {
    protected:
        Element* createElement();
        bool parseFeatures(YAML::Node node, Element* el);

    public:
        ParameterParser(map<boost::uuids::uuid, Element*>* elements) : TypedElementParser(elements){
            this->keyword = "parameter";
        }
        bool emit(YAML::Emitter& emitter, Element* el);
};

#endif
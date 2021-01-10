#ifndef MODELPARSER
#define MODELPARSER
#include "templateParser.h"
#include "namespaceParser.h"
#include "../../uml/headers/model.h"
using namespace std;

class ModelParser : public NamespaceParser {
    protected:
        Element* createElement();
        virtual bool parseFeatures(YAML::Node node, Element* el);

    public:
        ModelParser(map<boost::uuids::uuid, Element*>* elements) : NamespaceParser(elements){
            this->keyword = "model";
            // this->elements = elements;
        }
};
#endif
#ifndef MODELPARSER
#define MODELPARSER
#include "templateParser.h"
#include "namespaceParser.h"
#include "uml/model.h"
using namespace std;

class ModelParser : public NamespaceParser {
    protected:
        UML::Element* createElement();
        virtual bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        ModelParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamespaceParser(elements){
            this->keyword = "model";
            // this->elements = elements;
        }
};
#endif
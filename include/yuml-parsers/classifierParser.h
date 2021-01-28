#ifndef CLASSIFIERPARSER
#define CLASSIFIERPARSER
#include "namespaceParser.h"
#include "propertyParser.h"
#include "uml/classifier.h"
using namespace std;

class ClassifierParser: public NamespaceParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        ClassifierParser(map<boost::uuids::uuid, UML::Element*>* elements) : NamespaceParser(elements){
            // this->elements = elements;
        };
};
#endif
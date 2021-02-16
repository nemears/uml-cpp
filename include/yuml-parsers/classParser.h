#ifndef CLASSPARSER
#define CLASSPARSER
#include "classifierParser.h"
#include "uml/class.h"
using namespace std;

/**
 * Forward declaration of OperationParser to have it compile
 **/
class OperationParser;

class ClassParser : public ClassifierParser {
    protected:
        UML::Element* createElement() override;
        bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        ClassParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ClassifierParser(elements, postParsers){
            this->keyword = "class";    
        }

        bool emit(YAML::Emitter& emitter, Element* el) override;
        static ClassParser createNewParser();
};

#endif
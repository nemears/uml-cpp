#ifndef MODELPARSER
#define MODELPARSER

#include "packageParser.h"
#include "uml/model.h"
using namespace std;

class ModelParser : public PackageParser {
    protected:
        UML::Element* createElement();
        virtual bool parseFeatures(YAML::Node node, UML::Element* el);

    public:
        ModelParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            PackageParser(elements, postParsers),
            NamedElementParser(elements, postParsers) {
                NamespaceParser::keyword = "model";
            }

        bool emit(YAML::Emitter& emitter, Element* el);
        static ModelParser createNewParser();
        static void deleteParser(TemplateParser* parser);
};
#endif
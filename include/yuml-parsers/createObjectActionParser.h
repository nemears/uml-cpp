#ifndef CREATE_OBJECT_PARSER_H
#define CREATE_OBJECT_PARSER_H

#include "actionParser.h"
#include "uml/createObjectAction.h"

class CreateObjectActionParser : public ActionParser {
    protected:
        bool parseFeatures(YAML::Node node, Element* el) override;
    public:
        Element* createElement() override;
        bool emit(YAML::Emitter& emitter, Element* el) override;
        CreateObjectActionParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ActionParser(elements, postParsers), 
            NamedElementParser(elements, postParsers){
            this->keyword = "action";
        };
        static void setClassifierLater(YAML::Node node, Element* createObjectNode, Element* classifier) {
            dynamic_cast<CreateObjectAction*>(createObjectNode)->setClassifier(dynamic_cast<Classifier*>(classifier));
        };
};

#endif
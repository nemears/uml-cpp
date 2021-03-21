#ifndef ACTIONPARSER_H
#define ACTIONPARSER_H

#include "activityNodeParser.h"
#include "inputPinParser.h"
#include "outputPinParser.h"
#include "uml/action.h"

class ActionParser : public ActivityNodeParser {
    protected:
        bool parseFeatures(YAML::Node node, Element * el) override;
    
    public:
        Element* createElement() override;
        bool emit(YAML::Emitter& emitter, Element* el) override;
        ActionParser(map<boost::uuids::uuid, Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            ActivityNodeParser(elements, postParsers), 
            NamedElementParser(elements, postParsers){
            this->keyword = "action";
        };
        static void addInputPinLater(YAML::Node node, Element* action, Element* inputPin) {
            dynamic_cast<Action*>(action)->getInputs().add(*dynamic_cast<InputPin*>(inputPin));
            inputPin->setOwner(action);
        }
        static void addOutputPinLater(YAML::Node node, Element* action, Element* outputPin) {
            dynamic_cast<Action*>(action)->getOutputs().add(*dynamic_cast<OutputPin*>(outputPin));
            outputPin->setOwner(action);
        }
        static ActionParser createNewParser();
};

#endif
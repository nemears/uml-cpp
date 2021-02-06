#ifndef ELEMENTPARSER
#define ELEMENTPARSER
#include <list>
#include <iostream>
#include "templateParser.h"
using namespace std;

class ElementParser : public TemplateParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el);
    public:
        ElementParser(map<boost::uuids::uuid, UML::Element*>* elements) : TemplateParser(elements) {
            
        };

        virtual bool emit(YAML::Emitter& emitter, Element* el) override;

        // error for invalid element keyword
        class InvalidIdentifierException: public exception {

            private:
            string msg;

            public:
            InvalidIdentifierException(const int lineNumber, const string scalar) : 
                msg("Invalid identifier for config file at line number: " + to_string(lineNumber) + ", value: <" + scalar + ">\n")
                {}
            virtual const char* what() const throw() {
                return msg.c_str();
            }
        };

        // error for trying to emit abstract types
        class AbstractTypeEmitException : public exception {

            private:
            string msg;

            public:
            AbstractTypeEmitException(const string type, const string uuid) :
                msg("Abstract type " + type + " and uuid: " + uuid + " was attempted to be emit\n")
                {}
            virtual const char* what() const throw() {
                return msg.c_str();
            }
        };
};
#endif
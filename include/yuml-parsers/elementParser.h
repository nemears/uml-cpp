#ifndef ELEMENTPARSER
#define ELEMENTPARSER
#include <list>
#include <iostream>
#include "templateParser.h"
#include "uml/sequence.h"
#include "uml/relationship.h"

using namespace std;

class ElementParser : public TemplateParser {
    protected:
        bool parseFeatures(YAML::Node node, UML::Element* el) override;
    public:
        ElementParser(map<boost::uuids::uuid, UML::Element*>* elements, map<boost::uuids::uuid, PostParser*>* postParsers) : 
            TemplateParser(elements, postParsers) {
            
        };
        static void addRelationshipLater(YAML::Node node, Element* el, Element* relationship);
        bool emit(YAML::Emitter& emitter, Element* el) override;

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

        class InvalidNodeTypeException: public exception {

            private:
                string msg;
            public: 
                InvalidNodeTypeException(const int line, const string type) :
                    msg("Invalid node type on line " + to_string(line) + ", Expected node of type " + type + '\n')
                    {}
                virtual const char* what() const throw() {
                    return msg.c_str();
                }
        };
};
#endif
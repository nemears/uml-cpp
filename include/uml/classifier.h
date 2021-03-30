#ifndef CLASSIFIERH
#define CLASSIFIERH
#include <list>
#include "type.h"
#include "property.h"
#include "sequence.h"
#include "namespace.h"
using namespace std;

namespace UML{

    class Generalization;

    /**
     * A Classifier represents a classification of instances according to their Features
     **/
    class Classifier: public Namespace, public Type {
        protected:
            Sequence<Property>* m_attributes;
            Sequence<Generalization>* m_generalizations;
            Sequence<Classifier>* m_generals;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
            void reindexName(string oldName, string newName) override;
            class AddAttributeFunctor : public AbstractSequenceFunctor {
                public:
                    AddAttributeFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class CheckGeneralizationFunctor : public AbstractSequenceFunctor {
                public:
                    CheckGeneralizationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddGeneralizationFunctor : public AbstractSequenceFunctor {
                public:
                    AddGeneralizationFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class AddGeneralFunctor : public AbstractSequenceFunctor {
                public:
                    AddGeneralFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Classifier();
            ~Classifier();
            string getName() override;
            void setName(const string& name) override;
            Sequence<Property>& getAttributes();

            /**
             * Generalizations to inherited classifier
             * the generalizations specific reference is this classifier
             **/
            Sequence<Generalization>& getGeneralizations();
            /**
             * this returns all classifiers this classifier inherits from
             **/
            Sequence<Classifier>& getGenerals();
            ElementType getElementType() override;
            bool isPrimitive() override;
    };

    //Exceptions

    class InvalidGeneralizationException: public exception {
        private:
            string msg;
        public:
            InvalidGeneralizationException(string uuid) :
                msg("Invalid Generalization (uuid: " + uuid + ") was added to generalization set, only the Specific classifier may hold generalizations!")
                {};
            virtual const char* what() const throw() {
                return msg.c_str();
            };
    };
}
#endif
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
            Sequence<Generalization>& getGeneralizations();
            Sequence<Classifier>& getGenerals();
            ElementType getElementType() override;
            bool isPrimitive() override;
    };
}
#endif
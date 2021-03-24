#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "sequence.h"

namespace UML {
    class Relationship : public Element {
        protected:
            Sequence<>* m_relatedElements;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;

            class AddRelationshipFunctor : public AbstractSequenceFunctor {
                public:
                    AddRelationshipFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
        public:
            Relationship();
            ~Relationship();
            Sequence<>& getRelatedElements();
            ElementType getElementType() override;
    };

    //Exceptions

    class DuplicateRelationshipException: public exception {
        private:
            string msg;
        public:
            DuplicateRelationshipException(string uuid) :
                msg("Duplicate relationship (uuid: " + uuid + ") added to sequence specified as set!")
                {};
            virtual const char* what() const throw() {
                return msg.c_str();
            };
    };
}

#endif
#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "sequence.h"

namespace UML {
    class Relationship : virtual public Element {
        protected:
            Sequence<> m_relatedElements;
            void reindexID(ID oldID, ID newID) override;

            class AddRelationshipFunctor : public AbstractSequenceFunctor {
                public:
                    AddRelationshipFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class CheckRelatedElementsFunctor : public AbstractSequenceFunctor {
                public:
                    CheckRelatedElementsFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveRelatedElementsFunctor : public AbstractSequenceFunctor {
                public:
                    RemoveRelatedElementsFunctor(Element* me) : AbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void setManager(UmlManager* manager) override;
        public:
            Relationship();
            virtual ~Relationship();
            Sequence<>& getRelatedElements();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::RELATIONSHIP;
            };
    };

    //Exceptions

    class DuplicateRelatedElementException: public std::exception {
        private:
            std::string msg;
        public:
            DuplicateRelatedElementException(std::string uuid) :
                msg("Duplicate relationship (uuid: " + uuid + ") added to sequence specified as set!")
                {};
            virtual const char* what() const throw() {
                return msg.c_str();
            };
    };
}

#endif
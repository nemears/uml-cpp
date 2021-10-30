#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "sequence.h"

namespace UML {
    class Relationship : virtual public Element {
        protected:
            Sequence<> m_relatedElements = Sequence<>(this);
            class AddRelationshipFunctor : public TemplateAbstractSequenceFunctor<Element,Relationship> {
                public:
                    AddRelationshipFunctor(Relationship* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class CheckRelatedElementsFunctor : public TemplateAbstractSequenceFunctor<Element,Relationship> {
                public:
                    CheckRelatedElementsFunctor(Relationship* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            class RemoveRelatedElementsFunctor : public TemplateAbstractSequenceFunctor<Element,Relationship> {
                public:
                    RemoveRelatedElementsFunctor(Relationship* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Element& el) const override;
            };
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void restoreReferences() override;
            void referenceErased(ID id) override;
            Relationship();
        public:
            Relationship(const Relationship& relationship);
            virtual ~Relationship();
            Sequence<>& getRelatedElements();
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
#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "set.h"

namespace UML {
    class Relationship : virtual public Element {
        protected:
            Set<Element, Relationship> m_relatedElements = Set<Element, Relationship>(this);
            void referencingReleased(ID id) override;
            void referenceReindexed(ID oldID, ID newID) override;
            void reindexName(std::string oldName, std::string newName) override;
            void referenceErased(ID id) override;
            void init();
            void copy(const Relationship& rhs);
            Relationship();
        public:
            Relationship(const Relationship& relationship);
            virtual ~Relationship();
            Set<Element, Relationship>& getRelatedElements();
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
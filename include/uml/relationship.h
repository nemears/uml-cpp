#ifndef RELATIONSHIP_H
#define RELATIONSHIP_H

#include "sequence.h"

namespace UML {
    class Relationship : public Element {
        protected:
            Sequence<>* m_relatedElements;
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
        public:
            Relationship();
            ~Relationship();
            Sequence<>& getRelatedElements();
            ElementType getElementType() override;
    };
}

#endif
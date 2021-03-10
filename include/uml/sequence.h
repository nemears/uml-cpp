#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <map>
#include <boost/uuid/uuid.hpp>
#include "element.h"
#include "namedElement.h"

namespace UML {

    class SequenceIterator;

    class Sequence {
        friend class SequenceIterator;
        private:
            map<boost::uuids::uuid, Element*> m_data;
            map<string, NamedElement*> m_nameTranslation;
        public:
            void add(Element& el);
            void remove(Element& el);
            Element* get(boost::uuids::uuid id);
            Element* get(string name);
            SequenceIterator iterator();
    };

    class SequenceIterator {
        friend class Sequence;
        private:
            Sequence* m_sequence;
            map<boost::uuids::uuid, Element*>::iterator m_curr;
            map<boost::uuids::uuid, Element*>::iterator m_end;
            SequenceIterator(Sequence* seq) : m_sequence(seq){
                m_curr = m_sequence->m_data.begin();
                m_end = m_sequence->m_data.end();
            };
        public:
            Element* getNext();
            bool hasNext();
    };
}

#endif
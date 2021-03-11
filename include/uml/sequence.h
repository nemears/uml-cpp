#ifndef SEQUENCE_H
#define SEQUENCE_H

#include <map>
#include <vector>
#include <boost/uuid/uuid.hpp>
#include "element.h"
#include "namedElement.h"

namespace UML {

    class SequenceIterator;

    class Sequence {
        friend class SequenceIterator;
        private:
            map<boost::uuids::uuid, Element*> m_data;
            vector<boost::uuids::uuid> m_order;
            map<string, NamedElement*> m_nameTranslation;
        public:
            void add(Element& el);
            void remove(Element& el);
            size_t size();
            Element* get(boost::uuids::uuid id);
            Element* get(string name);
            SequenceIterator iterator();
    };

    class SequenceIterator {
        friend class Sequence;
        private:
            Sequence* m_sequence;
            vector<boost::uuids::uuid>::iterator m_curr;
            vector<boost::uuids::uuid>::iterator m_end;
            SequenceIterator(Sequence* seq) : m_sequence(seq){
                m_curr = m_sequence->m_order.begin();
                m_end = m_sequence->m_order.end();
            };
        public:
            Element* getNext();
            bool hasNext();
    };
}

#endif
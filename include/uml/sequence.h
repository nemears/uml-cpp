#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "namedElement.h"

namespace UML {
    /**
     * Sequence Class, Holds collections of uml elements
     **/
    template<class T> class SequenceIterator;
    template <class T = Element> class Sequence {
        friend class SequenceIterator<T>;
        private:
            map<boost::uuids::uuid, T*> m_data;
            vector<boost::uuids::uuid> m_order;
            map<string, T*> m_nameTranslation;
        public:

            // Methods
            void add(T& el) {
                if (!m_data.count(el.getID())) {
                    m_data[el.getID()] = &el;
                    m_order.push_back(el.getID());
                    if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                        m_nameTranslation[dynamic_cast<NamedElement*>(&el)->getName()] = &el;
                    }
                } else {
                    m_order.push_back(el.getID());
                }
            };
            void remove(T& el) {
                if (m_data.count(el.getID())) {
                    // erase element in uuid map
                    m_data.erase(el.getID());

                    // erase element in name map
                    if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                        if (!dynamic_cast<NamedElement*>(&el)->getName().empty()) {
                            m_nameTranslation.erase(dynamic_cast<NamedElement*>(&el)->getName());
                        }
                    }

                    // erase all uuids in order
                    m_order.erase(std::remove(m_order.begin(), m_order.end(), el.getID()), m_order.end());
                } else {
                    throw ElementDoesntExistException(el);
                }
            };
            size_t size() { return m_order.size(); };
            bool empty() { return m_order.empty(); };
            T* get(boost::uuids::uuid id) { return m_data[id]; };
            T* get(string name) { return m_nameTranslation[name]; };
            T* get(size_t index) { return m_data[m_order[index]]; };
            T* front() { return m_data[m_order.front()]; };
            T* back() { return m_data[m_order.back()]; }
            vector<T*> copyToVector() { 
                vector<T*> ret;
                for (auto const& id : m_order) {
                    ret.push_back(m_data[id]);
                }
                ret.shrink_to_fit();
                return ret;
            };
            SequenceIterator<T> iterator() { return SequenceIterator<T>(this); };
    };


    // Sequence Iterator
    template <class T = Element> class SequenceIterator {
        friend class Sequence<T>;
        private:
            Sequence<T>* m_sequence;
            vector<boost::uuids::uuid>::iterator m_curr;
            vector<boost::uuids::uuid>::iterator m_end;
            vector<T*> m_iter;
            SequenceIterator(Sequence<T>* seq) : m_sequence(seq){
                m_curr = m_sequence->m_order.begin();
                m_end = m_sequence->m_order.end();
                for (auto const& id : m_sequence->m_order) {
                    m_iter.push_back(m_sequence->m_data[id]);
                }
                m_iter.shrink_to_fit();
            };
        public:
            T* getNext() {
                Element* ret = m_sequence->m_data[(*m_curr)];
                ++m_curr;
                return ret;
            };
            bool hasNext() { return m_curr != m_end; };
            typename vector<T*>::iterator begin() { return m_iter.begin(); };
            typename vector<T*>::iterator end() { return m_iter.end(); };
    };
}

#endif
#ifndef SEQUENCE_H
#define SEQUENCE_H
#include "namedElement.h"

namespace UML {
    /**
     * Sequence Class, Holds collections of uml elements
     **/
    template <class T = Element> class Sequence {
        friend class Element;
        private:
            map<boost::uuids::uuid, T*> m_data;
            vector<boost::uuids::uuid> m_order;
            map<string, T*> m_nameTranslation;
            vector<T*> m_rep;
            void reindex(boost::uuids::uuid oldID, boost::uuids::uuid newID) {

                // m_data
                T* temp = m_data[oldID];
                m_data.erase(oldID);
                m_data[newID] = temp;

                // m_order
                std::replace(m_order.begin(), m_order.end(), oldID, newID);
            }
        public:

            // Methods
            void add(T& el) {
                if (!m_data.count(el.getID())) {
                    m_data[el.getID()] = &el;
                    m_order.push_back(el.getID());
                    m_rep.push_back(&el);
                    if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                        m_nameTranslation[dynamic_cast<NamedElement*>(&el)->getName()] = &el;
                    }
                } else {
                    m_order.push_back(el.getID());
                    m_rep.push_back(&el);
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
                    m_rep.erase(std::remove(m_rep.begin(), m_rep.end(), &el), m_rep.end());
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
            typename vector<T*>::iterator begin() { return m_rep.begin(); };
            typename vector<T*>::iterator end() { return m_rep.end(); };
    };
}

#endif
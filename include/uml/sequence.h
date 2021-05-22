#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "namedElement.h"

namespace UML {

    namespace Parsers {
        class ParserMetaData;
    }

    class AbstractSequenceFunctor {
        protected:
            Element* m_el;
        public:
            AbstractSequenceFunctor(Element* me) {
                m_el = me;
            };
            virtual void operator()(Element& el) const = 0;
    };

    class Property;
    class Slot;
    class BehavioralFeature;
    class Operation;
    class ActivityNode;
    class ActivityEdge;
    class OutputPin;
    class InputPin;
    class ValueSpecification;
    class Parameter;
    class Relationship;
    class DirectedRelationship;
    class Classifier;
    class Namespace;
    class Class;
    class InstanceSpecification;
    class Behavior;
    class Expression;
    class Activity;
    class Action;
    class Package;
    class Feature;
    class DataType;
    class StructuredClassifier;
    class Association;
    class Enumeration;
    class AddOwnedCommentFunctor;
    class RemoveOwnedCommentFunctor;
    class AddDirectedRelationshipFunctor;
    class RemoveDirectedRelationshipFunctor;
    class Generalization;
    /**
     * Sequence Class, Holds collections of uml elements
     **/
    template <class T> class Sequence {
        friend class Parsers::ParserMetaData;
        friend class Element;
        friend class NamedElement;
        friend class Property;
        friend class Slot;
        friend class BehavioralFeature;
        friend class Operation;
        friend class ActivityNode;
        friend class ActivityEdge;
        friend class OutputPin;
        friend class InputPin;
        friend class ValueSpecification;
        friend class Parameter;
        friend class Relationship;
        friend class DirectedRelationship;
        friend class Classifier;
        friend class Namespace;
        friend class Class;
        friend class InstanceSpecification;
        friend class Behavior;
        friend class Expression;
        friend class Activity;
        friend class Action;
        friend class Package;
        friend class Feature;
        friend class DataType;
        friend class StructuredClassifier;
        friend class Association;
        friend class Enumeration;
        friend class Comment;
        friend class AddOwnedCommentFunctor;
        friend class RemoveOwnedCommentFunctor;
        friend class AddDirectedRelationshipFunctor;
        friend class RemoveDirectedRelationshipFunctor;
        friend class Generalization;
        private:
            std::map<boost::uuids::uuid, T*> m_data;
            std::vector<boost::uuids::uuid> m_order;
            std::map<std::string, T*> m_nameTranslation;
            std::vector<T*> m_rep;
            std::vector<AbstractSequenceFunctor*> addProcedures;
            std::vector<AbstractSequenceFunctor*> addChecks;
            std::vector<AbstractSequenceFunctor*> removeProcedures;
            std::vector<AbstractSequenceFunctor*> removeChecks;
            void reindex(boost::uuids::uuid oldID, boost::uuids::uuid newID) {

                // m_data
                T* temp = m_data[oldID];
                m_data.erase(oldID);
                m_data[newID] = temp;

                // m_order
                std::replace(m_order.begin(), m_order.end(), oldID, newID);
            };
            void reindex(boost::uuids::uuid elID, std::string oldName, std::string newName) {

                // m_nameTranslation
                T* temp = m_data[elID];
                if (m_nameTranslation[newName]) {
                    // TODO error throw duplicate name exception
                }
                m_nameTranslation.erase(oldName);
                m_nameTranslation[newName] = temp;
            }

            // add without checks
            void internalAdd(T& el) {
                if (!m_data.count(el.getID())) {
                    m_data[el.getID()] = &el;
                    m_order.push_back(el.getID());
                    m_rep.push_back(&el);
                    if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                        if (dynamic_cast<NamedElement*>(&el)->getName().length() > 0) {
                            m_nameTranslation[dynamic_cast<NamedElement*>(&el)->getName()] = &el;
                        }
                    }
                } else {
                    m_order.push_back(el.getID());
                    m_rep.push_back(&el);
                }

                for (auto const& fun : addProcedures) {
                    (*fun)(el);
                }
            }

            // remove without checks
            void internalRemove(T& el) {
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
                    for (auto const& fun : removeProcedures) {
                        (*fun)(el);
                    }
                } else {
                    throw ElementDoesntExistException(el);
                }
            }
        public:

            // destructor
            ~Sequence<T>() {
                for (auto const& addProc: addProcedures) {
                    delete addProc;
                }

                for (auto const& addCheck: addChecks) {
                    delete addCheck;
                }

                for (auto const& remProc: removeProcedures) {
                    delete remProc;
                }

                for (auto const& remCheck: removeChecks) {
                    delete remCheck;
                }
            }

            // Methods
            void add(T& el) {
                for (auto const& fun : addChecks) {
                    (*fun)(el);
                }

                internalAdd(el);
            };
            void remove(T& el) {
                for (auto const& fun : removeChecks) {
                    (*fun)(el);
                }

                internalRemove(el);
            };
            size_t size() { return m_order.size(); };
            bool empty() { return m_order.empty(); };
            T* get(boost::uuids::uuid id) { return m_data[id]; };
            size_t count(boost::uuids::uuid id) { return m_data.count(id); };
            T* get(std::string name) { return m_nameTranslation[name]; };
            T* get(size_t index) { return m_data[m_order[index]]; };
            T* front() { return m_data[m_order.front()]; };
            T* back() { return m_data[m_order.back()]; }
            typename std::vector<T*>::iterator begin() { return m_rep.begin(); };
            typename std::vector<T*>::iterator end() { return m_rep.end(); };
    };

    class ReadOnlySequenceException : public std::exception {
        friend class Element;
        private:
            std::string msg;

        public:
            ReadOnlySequenceException(std::string elID, std::string nameOfSequence) : 
                msg("Sequence " + nameOfSequence + " for element uuid: " + elID + " is read only!")
                {}
            virtual const char* what() const throw() {
                return msg.c_str();
            }
    };
}

#endif
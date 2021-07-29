#ifndef SEQUENCE_H
#define SEQUENCE_H

#include "namedElement.h"
#include "umlManager.h"

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
    class AddRelationshipFunctor;
    class RemoveRelationshipFunctor;
    class RedefinableElement;
    class TemplateableElement;
    class TemplateSignature;
    class TemplateParameter;
    class TemplateParameterSubstitution;
    class TemplateBinding;
    class AddAppliedStereotypeFunctor;
    class RemoveAppliedStereotypeFunctor;
    class Dependency;
    class Deployment;
    class DeploymentTarget;
    class Artifact;

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

    template <class T = Element> struct SequenceIterator;

    // new sequence implementation
    template <class T> class Sequence {
        friend class SequenceIterator<T>;
        friend class UmlManager;
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
        friend class AddRelationshipFunctor;
        friend class RemoveRelationshipFunctor;
        friend class RedefinableElement;
        friend class TemplateableElement;
        friend class TemplateSignature;
        friend class TemplateParameter;
        friend class TemplateParameterSubstitution;
        friend class TemplateBinding;
        friend class AddAppliedStereotypeFunctor;
        friend class RemoveAppliedStereotypeFunctor;
        friend class Dependency;
        friend class Deployment;
        friend class DeploymentTarget;
        friend class Artifact;
        
        private:
            // Manager
            UmlManager* m_manager = 0;

            // Data
            std::vector<ID> m_order;
            std::unordered_map<ID, T*> m_rep;
            std::unordered_map<std::string, ID> m_names;

            // Functors
            std::vector<AbstractSequenceFunctor*> addProcedures;
            std::vector<AbstractSequenceFunctor*> addChecks;
            std::vector<AbstractSequenceFunctor*> removeProcedures;
            std::vector<AbstractSequenceFunctor*> removeChecks;

            void reindex(ID oldID, ID newID) {

                // m_data
                T* temp = m_rep[oldID];
                if (!temp) {
                    temp = &m_manager->get<T>(oldID);
                }
                m_rep.erase(oldID);
                m_rep[newID] = temp;

                // m_names
                if (temp->isSubClassOf(ElementType::NAMED_ELEMENT)) {
                    if (!temp->template as<NamedElement>().getName().empty()) {
                        m_names[temp->template as<NamedElement>().getName()] = newID;
                    }
                }

                // m_order
                std::replace(m_order.begin(), m_order.end(), oldID, newID);
            };

            void reindex(ID id, std::string oldName, std::string newName) {
                if (!oldName.empty()) {
                    m_names.erase(oldName);
                }
                m_names[newName] = id;
            }

            // internal functions
            void internalAdd(T& el) {
                m_order.push_back(el.getID());

                if (m_manager) {
                    // set element to null until it is accessed
                    m_rep[el.getID()] = 0;
                } else {
                    // no manager, set it to element
                    m_rep[el.getID()] = &el;
                }

                if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                    if (el.template as<NamedElement>().getName().size() > 0) {
                        m_names[el.template as<NamedElement>().getName()] = el.getID();
                    }
                }

                // apply procedures
                for (auto const& fun : addProcedures) {
                    (*fun)(el);
                }
            };

            void internalRemove(T& el) {
                m_order.erase(std::remove(m_order.begin(), m_order.end(), el.getID()), m_order.end()) - m_order.begin();
                m_rep.erase(el.getID());
                if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                    m_names.erase(el.template as<NamedElement>().getName());
                }

                // apply procedures
                for (auto const& fun : removeProcedures) {
                    (*fun)(el);
                }
            };
        public:
            // Destructor

            ~Sequence<T>() {
                for (auto const& fun : addChecks) {
                    delete fun;
                }

                for (auto const& fun : removeChecks) {
                    delete fun;
                }

                for (auto const& fun : addProcedures) {
                    delete fun;
                }

                for (auto const& fun : removeProcedures) {
                    delete fun;
                }
            }

            // Mutators

            void add(T& el) {
                for (auto const& fun : addChecks) {
                    (*fun)(el);
                }

                internalAdd(el);
            };
            void remove(T& el) {
                if (m_rep.count(el.getID())) {
                    for (auto const& fun : removeChecks) {
                        (*fun)(el);
                    }

                    internalRemove(el);
                } else {
                    throw ElementDoesntExistException(el);
                }
            };

            // Accessors

            size_t size() { return m_order.size(); };
            bool empty() { return m_order.empty(); };
            T* get(ID id) { 
                if (m_rep.count(id)) {
                    if (!m_rep[id]) {
                        m_rep[id] = &m_manager->get<T>(id);
                    }
                    return m_rep[id];
                }
                return 0;
            };
            T* get(size_t index) {
                if (!m_rep[m_order.at(index)]) {
                    m_rep[m_order.at(index)] = &m_manager->get<T>(m_order.at(index));
                }
                return m_rep[m_order.at(index)];
            };
            T* get(std::string name) {
                if (m_names.count(name)) {
                    if (!m_rep[m_names[name]]) {
                        m_rep[m_names[name]] = &m_manager->get<T>(m_names[name]);
                    }
                    return m_rep[m_names[name]];
                }
                return 0;
            };
            T* front() { 
                if (!m_rep[m_order.front()]) {
                    m_rep[m_order.front()] = &m_manager->get<T>(m_order.front());
                }
                return m_rep[m_order.front()];
            };
            T* back() {
                if (!m_rep[m_order.back()]) {
                    m_rep[m_order.back()] = &m_manager->get<T>(m_order.back());
                }
                return m_rep[m_order.back()];
            }
            size_t count(ID id) { return m_rep.count(id); };

            SequenceIterator<T> begin() { return SequenceIterator(this, m_order.begin()); };
            SequenceIterator<T> end() { return SequenceIterator(this, m_order.end()); };
    };

    template <class T> struct SequenceIterator {
        private:
            T* m_ptr;
            Sequence<T>* m_sequence;
            std::vector<ID>::iterator m_orderIt;
        
        public:
            SequenceIterator<T>(Sequence<T>* sequence, std::vector<ID>::iterator orderIt) {
                m_sequence = sequence;
                m_orderIt = orderIt;
                if (m_orderIt != m_sequence->m_order.end()) {
                    m_ptr = sequence->get(*m_orderIt);
                }
            };
            T& operator*() { return *m_ptr; };
            T* operator->() { return m_ptr; };

            // TODO: throw exception if iterator is increased and sequence size has increased, or do complicated handling 
            SequenceIterator operator++() {
                ++m_orderIt;
                if (m_orderIt != m_sequence->m_order.end()) {
                    m_ptr = m_sequence->m_rep.at(*m_orderIt);
                    if (!m_ptr) {
                        m_ptr = &m_sequence->m_manager->template get<T>(*m_orderIt);
                    }
                }
                return *this;
            };

            SequenceIterator operator++(int) {
                ++m_orderIt;
                if (m_orderIt != m_sequence->m_order.end()) {
                    m_ptr = m_sequence->m_rep.at(*m_orderIt);
                    if (!m_ptr) {
                        m_ptr = &m_sequence->m_manager->template get<T>(*m_orderIt);
                    }
                }
                SequenceIterator ret = *this;
                return ret;
            };

            friend bool operator== (const SequenceIterator& a, const SequenceIterator& b) { return a.m_orderIt == b.m_orderIt; };
            friend bool operator!= (const SequenceIterator& a, const SequenceIterator& b) { return a.m_orderIt != b.m_orderIt; };
    };
}

#endif
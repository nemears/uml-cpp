#ifndef ELEMENTH
#define ELEMENTH
#include <string>
#include <list>
#include <regex>
#include <exception>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

using namespace std;

namespace UML {

    // Element Type enum to get the type of object on runtime
    enum class ElementType {
        ACTION,
        ACTIVITY,
        ACTIVITY_EDGE,
        ACTIVITY_NODE,
        BEHAVIOR,
        CALL_BEHAVIOR_ACTION,
        CLASS,
        CLASSIFIER,
        CONTROL_FLOW,
        CREATE_OBJECT_ACTION,
        DECISION_NODE,
        ELEMENT,
        EXPRESSION,
        FINAL_NODE,
        FORK_NODE,
        INITIAL_NODE,
        INPUT_PIN,
        INSTANCE_SPECIFICATION,
        INSTANCE_VALUE,
        JOIN_NODE,
        LITERAL_BOOL,
        LITERAL_INT,
        LITERAL_REAL,
        LITERAL_STRING,
        MERGE_NODE,
        MODEL,
        MULTIPLICITY_ELEMENT,
        NAMED_ELEMENT,
        NAMESPACE,
        OBJECT_FLOW,
        OBJECT_NODE,
        OPAQUE_BEHAVIOR,
        OPERATION,
        OUTPUT_PIN,
        PARAMETER,
        PARAMETER_NODE,
        PIN,
        PRIMITIVE_TYPE,
        PROPERTY,
        SLOT,
        STRUCTURAL_FEATURE,
        TYPE,
        TYPED_ELEMENT,
        VALUE_SPECIFICATION
    };

    // Forward Declarations for Sequence
    class Element;
    class NamedElement;
    class ElementAlreadyExistsException;
    class ElementDoesntExistException;
    template <class T> class SequenceIterator;

    /**
     * Sequence Class, Holds collections of uml elements
     **/
    template <class T = Element> class Sequence {
        friend class SequenceIterator<T>;
        private:
            map<boost::uuids::uuid, T*> m_data;
            vector<boost::uuids::uuid> m_order;
            map<string, T*> m_nameTranslation;
        public:

            // Methods
            void add(T& el) {
                if (!m_data.count(el.uuid)) {
                    m_data[el.uuid] = &el;
                    m_order.push_back(el.uuid);
                    if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                        m_nameTranslation[dynamic_cast<NamedElement*>(&el)->getName()] = &el;
                    }
                } else {
                    throw ElementAlreadyExistsException(el);
                }
            };
            void remove(T& el) {
                if (m_data.count(el.uuid)) {
                    m_data.erase(el.uuid);
                    vector<boost::uuids::uuid>::iterator orderIt = m_order.begin();
                    while((*orderIt) != el.uuid && orderIt != m_order.end()) {
                        ++orderIt;
                    }
                    if ((*orderIt) == el.uuid) {
                        m_order.erase(orderIt);
                    } else {
                        throw ElementDoesntExistException(el);
                    }
                    if (el.isSubClassOf(ElementType::NAMED_ELEMENT)) {
                        m_nameTranslation.erase(dynamic_cast<NamedElement*>(&el)->getName());
                    }
                } else {
                    throw ElementDoesntExistException(el);
                }
            };
            size_t size() { return m_data.size(); };
            T* get(boost::uuids::uuid id) { return m_data[id]; };
            T* get(string name) { return m_nameTranslation[name]; };
            SequenceIterator<T> iterator() { return SequenceIterator<T>(this); };
    };


    // Sequence Iterator
    template <class T = Element> class SequenceIterator {
        friend class Sequence<T>;
        private:
            Sequence<T>* m_sequence;
            vector<boost::uuids::uuid>::iterator m_curr;
            vector<boost::uuids::uuid>::iterator m_end;
            SequenceIterator(Sequence<T>* seq) : m_sequence(seq){
                m_curr = m_sequence->m_order.begin();
                m_end = m_sequence->m_order.end();
            };
        public:
            T* getNext() {
                Element* ret = m_sequence->m_data[(*m_curr)];
                ++m_curr;
                return ret;
            };
            bool hasNext() { return m_curr != m_end; };
    };

    // Helper function to assess possible uuids
    static bool isValidUUID4(string strn) {
        return regex_match (strn, regex("[0-9a-f]{8}-[0-9a-f]{4}-4[0-9a-f]{3}-[89ab][0-9a-f]{3}-[0-9a-f]{12}"));
    }

    /**
     * Element is the base class of all UML classes
     * It has three main attributes
     * owner - the element that has ownership of this element
     * ownedElements - the elements that this element owns
     * id - the elements unique id for quick comparison and indexing
     **/
    class Element {
        protected:
            Element* owner;
            Sequence<> m_ownedElements;

        public:
            list<Element*> ownedElements;
            boost::uuids::uuid uuid;
            Element() {
                uuid = boost::uuids::random_generator()();
                owner = NULL;
            };
            virtual ~Element() {};
            virtual void setID(string id);
            class InvalidID_Exception: public exception {
                public:
                virtual const char* what() const throw() {
                    return "String of id is not a valid UUID4";
                }
            } invalidID_Exception;
            virtual ElementType getElementType();
            bool isSubClassOf(ElementType eType);
            virtual string getElementTypeString();
            virtual string getIDstring();
            Element* getOwner();
            void setOwner(Element* owner);
    };

    //Exceptions
    class ElementAlreadyExistsException : public exception {
        private:
            string msg;

        public:
            ElementAlreadyExistsException(const Element& el) : 
                msg("Added element that was already in the Sequence\nuuid: " + boost::lexical_cast<string>(el.uuid))
                {}
            virtual const char* what() const throw() {
                return msg.c_str();
            }
    };
    class ElementDoesntExistException : public exception {
        private:
            string msg;

        public:
            ElementDoesntExistException(const Element& el) : 
                msg("Removed element that isn't in the Sequence\nuuid: " + boost::lexical_cast<string>(el.uuid))
                {}
            virtual const char* what() const throw() {
                return msg.c_str();
            }
    };
}
#endif
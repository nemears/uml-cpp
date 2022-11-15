#ifndef _UML_MANAGERS_ABSTRACT_MANAGER_H_
#define _UML_MANAGERS_ABSTRACT_MANAGER_H_

#include "uml/element.h"
#include "uml/set/setLock.h"

namespace UML {

    class AbstractUmlPtr;
    class AbstractSet;

    class AbstractManager {

        template <class T, class U> friend class Set;
        template <class T, class U, class AllocationPolicy> friend class PrivateSet;
        template <class T> friend struct SetIterator;
        template <class T, class U> friend struct OrderedSetIterator;
        template <class T, class U> friend class OrderedSet;
        template <class T, class U> friend class OrderedSet2;
        template <class T> friend class UmlPtr;
        template <class T, class U> friend class Singleton;
        template <class T, class U> friend class Singleton2;

        public:
            virtual Element* create(ElementType type) = 0;
            template <class T = Element>
            UmlPtr<T> create() {
                return UmlPtr<T>(&create(T::elementType())->template as<T>());
            }
            virtual ElementPtr get(ID id) = 0;
            virtual bool loaded(ID id) = 0;
            virtual void reindex (ID oldID, ID newID) = 0;
            virtual void release(Element& el) = 0;
            virtual void removeNode(ID id) = 0;
            virtual void erase(Element& el) = 0;
            virtual ElementPtr open(std::string path) = 0;
            virtual ElementPtr open() = 0;
            virtual ElementPtr getRoot() const = 0;
            virtual void setRoot(Element* root) = 0;
            virtual std::string getLocation(ID id) = 0;
            virtual std::string getLocation() = 0;
            virtual void setLocation(ID id, std::string location) = 0;
            virtual void save(std::string location) = 0;
            virtual void save() = 0;
        protected:
            virtual Element* get(Element* me, ID theID);
            virtual void removePtr(AbstractUmlPtr& ptr) = 0;
            virtual void destroyPtr(AbstractUmlPtr& ptr) = 0;
            virtual void assignPtr(AbstractUmlPtr& ptr) = 0;
            virtual void restorePtr(AbstractUmlPtr& ptr) = 0;
            virtual void addToSet(Element& el, AbstractSet& set) = 0;
            virtual void removeFromSet(ID id, AbstractSet& set) = 0;
            virtual SetLock lockEl(Element& el) = 0;
    };

    class ManagerStateException : public std::exception {
        std::string m_msg;
        public:
            ManagerStateException(){};
            ManagerStateException(std::string msg) : m_msg("Uml Manager bad state! " + msg) {};
            const char* what() const throw() override {
                return m_msg.c_str();
            };
    };
}

#endif
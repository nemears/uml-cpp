#ifndef _UML_MANAGERS_ABSTRACT_MANAGER_H_
#define _UML_MANAGERS_ABSTRACT_MANAGER_H_

#include "uml/element.h"

namespace UML {
    class AbstractManager {

        template <class T, class U> friend class Set;
        template <class T> friend struct SetIterator;
        template <class T, class U> friend struct OrderedSetIterator;

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
            Element* get(Element* me, ID theID);
    };

    class ManagerStateException : public std::exception {
        std::string m_msg;
        const char* what() const throw() override {
            return m_msg.c_str();
        };
        public:
            ManagerStateException(){};
            ManagerStateException(std::string msg) : m_msg("Uml Manager bad state! " + msg) {};
    };
}

#endif
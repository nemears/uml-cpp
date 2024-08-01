#pragma once

#include "uml/types/element.h"

namespace UML {

    class AbstractUmlPtr;
    class AbstractSet;

    class AbstractManager {

        template <class T, class DataTypePolicy, class ApiPolicy, class U> friend class PrivateSet;
        template <class T> friend class UmlPtr;
        friend class Element;

        public:
            virtual Element* create(ElementType type) = 0;
            template <class T>
            UmlPtr<T> create() {
                return UmlPtr<T>(&create(T::elementType())->template as<T>());
            }
            virtual ElementPtr get(ID id) = 0;
            virtual bool loaded(ID id) = 0;
            virtual void reindex (ID oldID, ID newID) = 0;
            virtual void release(Element& el) = 0;
            virtual void erase(Element& el) = 0;
            virtual bool exists(ID id) = 0;
            virtual ElementPtr open(std::string path) = 0;
            virtual ElementPtr open() = 0;
            virtual ElementPtr getRoot() const = 0;
            virtual void setRoot(Element* root) = 0;
            virtual void save(std::string location) = 0;
            virtual void save() = 0;
        protected:
            virtual Element* get(Element* me, ID theID);
            virtual ElementPtr createPtr(ID id) = 0;
            virtual void removePtr(AbstractUmlPtr& ptr) = 0;
            virtual void destroyPtr(AbstractUmlPtr& ptr) = 0;
            virtual void assignPtr(AbstractUmlPtr& ptr) = 0;
            virtual void restorePtr(AbstractUmlPtr& ptr) = 0;
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

#pragma once

#include "uml/types/element.h"

namespace UML {


    class AbstractManager {

        template <class T, class DataTypePolicy, class ApiPolicy, class U> friend class PrivateSet;
        template <class T, class U, class ApiPolicy>
        friend class Singleton;
        template <class T, class U, class ApiPolicy>
        friend class Set;
        template <class T> friend class UmlPtr;
        template <class T, class U, class ApiPolicy>
        friend class OrderedSet;
        friend class Element;
        friend class NamedElement;
        friend void parsePackageFeatures(YAML::Node node, Package& pckg, ParserData& data);

        public:
            virtual ElementPtr create(std::size_t elementType) = 0;
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
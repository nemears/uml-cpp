#pragma once

#include "uml/id.h"

namespace UML {

    template <class T>
    class UmlPtr;
    class AbstractElement;
    typedef UmlPtr<AbstractElement> AbstractElementPtr;
    struct ManagerNode;

    class  AbstractManager {

        friend struct ManagerNode;
        friend class AbstractElement;
        
        public:
            virtual AbstractElementPtr create(std::size_t elementType) = 0;
            virtual AbstractElementPtr createPtr(ID id) = 0;
            virtual AbstractElementPtr get(ID id) = 0;
            virtual void release (AbstractElement& el) = 0;
        protected:
            virtual void reindex(ID oldID, ID newID) = 0;
            virtual void destroy(ID id) = 0;
            
            // TODO rest of funcionality interface
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

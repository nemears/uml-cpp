#ifndef UML_CPP_CPP_PARSER_H
#define UML_CPP_CPP_PARSER_H
#include "uml/package.h"
#include "uml/umlManager.h"

namespace UML {
    namespace CPP {
        // WARN: must load cppProfile.yml beforehand 
        // TODO: swith to Artifact not Package when implemented
        Package* parseHeader(std::string path, UmlManager& manager);

        class UmlCppParserException : public std::exception {
            private:
                std::string m_msg;
            public:
                UmlCppParserException(const std::string msg) : m_msg(msg){};
                const char* what() const throw() override {
                    return m_msg.c_str();
                }
        };
    }
}

#endif
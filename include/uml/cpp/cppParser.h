#ifndef UML_CPP_CPP_PARSER_H
#define UML_CPP_CPP_PARSER_H

#include "uml/umlManager.h"
#include "uml/package.h"

namespace UML {
    namespace CPP {

        namespace Profile {
            static const ID cppProfileID = ID::fromString("CPP_4zpFq9s6YilFjqZAPguiluqk"); 
            static const ID cppNamespaceID = ID::fromString("Cpp_NAMESPACE_3FloKgLhiH2P0t");
            static const ID cppConstArrayID = ID::fromString("Cpp_CONST_ARRAY_Nw3c30z1PCo3");
            static const ID cppPointerID = ID::fromString("Cpp_POINTER_fHXNFR8qvi_PlTVD");
            static const ID cppReferenceID = ID::fromString("Cpp_REFERENCE_0NJ3ZsXRcBV33A");
            static const ID cppBoolID = ID::fromString("C_bool_sWBeSxCp5A7Ns9OJ4tBdG");
            static const ID cppIntID = ID::fromString("C_int_ZvgWKuxGtKtjRQPMNTXjic");
            static const ID cppCharID = ID::fromString("C_char_bvN6xdQ&&LaR7MU_F_9uR");
            static const ID cppFloatID = ID::fromString("C_float_FRQyo8d1KEQQLOnnPPn6");
            static const ID cppDoubleID = ID::fromString("C_double_HM2asoTiFmoWEK8ZuAE");
        }

        // WARN: must load cppProfile.yml beforehand 
        Package* parseHeader(std::string path, UmlManager& manager);
        void parseCpp(std::string name, std::vector<std::string> headers, std::vector<std::string> sourceFiles, UmlManager& manager);

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
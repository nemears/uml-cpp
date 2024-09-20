#pragma once

#include "typedElement.h"
#include "multiplicityElement.h"
#include "feature.h"

namespace UML {
    class StructuralFeature : virtual public TypedElement, public MultiplicityElement, public Feature {
        protected:
            bool m_readOnly = false;
            StructuralFeature(std::size_t elementType, AbstractManager& manager);
        public:
            class InvalidValueException: public std::exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to assign value that does not match structural features corresponded type";
                    }
            } invalidValueException;
            bool isReadOnly() const;
            void setReadOnly(bool readOnly);
            typedef TypeInfo<std::tuple<TypedElement, MultiplicityElement, Feature>, StructuralFeature> Info;
    };

    template <>
    struct ElementInfo<StructuralFeature> : public DefaultInfo {
        inline static std::string name{"StructuralFeature"};
        static const bool extraData = true;
        struct StructuralFeautreReadOnlyPolicy : public AbstractDataPolicy {
            StructuralFeature& el;
            StructuralFeautreReadOnlyPolicy(StructuralFeature& el) : el(el) {}
            std::string getData() override {
                if (el.isReadOnly()) {
                    return "true";
                }
                return "";
            }
            void setData(std::string data) override {
                if (data == "true") {
                    el.setReadOnly(true);
                }
                
            }
        };
        static DataList data(StructuralFeature& el) {
            return DataList {
                createDataPair("readonly", new StructuralFeautreReadOnlyPolicy(el))
            };
        }
    };
}

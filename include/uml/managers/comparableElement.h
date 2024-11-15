#pragma once

#include "uml/managers/abstractElement.h"
#include "uml/managers/templateTypeList.h"
#include <exception>

namespace UML {

    const struct AsException : public std::exception {
        const char* what() const noexcept override {
            return "Can not convert element to that type!";
        }
    } asException;

    template <class TypePolicyList>
    class ComparableElement : public AbstractElement {
        protected:
            // constructor
            ComparableElement(std::size_t elementType, AbstractManager& manager) : AbstractElement(elementType, manager) {}
        public:
            // is function to compare types compile time O(1)
            template <template <class> class T, std::size_t I = 0>
            constexpr bool is() const {
                if constexpr (TemplateTypeListIndex<T, TypePolicyList>::result == m_elementType) {
                    return true;
                }
                using TBases = T<ComparableElement>::Info::BaseList;
                if constexpr (I < TemplateTypeListSize<TBases>::result) {
                    if constexpr (TemplateTypeListType<I, TBases>::template result<ComparableElement>::template is<T>()) {
                        return true;
                    }
                }
                return is<T, I + 1>();
            }

            // as to cast to other managed types
            template <template<class> class T>
            constexpr T<ComparableElement>& as() const {
                if constexpr (is<T>()) {
                    return dynamic_cast<T<ComparableElement>&>(*this);
                }
                throw asException;
            }
    };
}

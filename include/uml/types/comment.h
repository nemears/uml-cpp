#pragma once

#include "egm/egm-basic.h"

namespace UML {
    template <class>
    class Element;

    template <class ManagerPolicy>
    class Comment : public ManagerPolicy {
        public:
            using Info = EGM::TypeInfo<Comment, EGM::TemplateTypeList<Element>>;
        protected:
            EGM::Set<Element, Comment> m_annotatedElement = EGM::Set<Element, Comment>(this);
            std::string m_body = "";
        private:
            void init() {}
        public:
            MANAGED_ELEMENT_CONSTRUCTOR(Comment);
            EGM::Set<Element, Comment>& getAnnotatedElements() {
                return m_annotatedElement;
            }
            std::string getBody() const {
                return m_body;
            }
            void setBody(std::string body) {
                m_body = body;
            }
    };
}

namespace EGM {
    template <>
    struct ElementInfo<UML::Comment> {
        static std::string name() { return "Comment"; }
        template <class Policy>
        struct BodyPolicy : public EGM::AbstractDataPolicy {
            ManagedPtr<UML::Comment<Policy>> el;
            BodyPolicy(UML::Comment<Policy>& ref) : el(&ref) {}
            std::string getData() override { return el->getBody(); }
            void setData(std::string data) override { el->setBody(data); }
        };
        template <class Policy>
        static DataList data(UML::Comment<Policy>& el) {
            return DataList {
                createDataPair<BodyPolicy<Policy>>("body", el)
            };
        }
        template <class Policy>
        static SetList sets(UML::Comment<Policy>& el) {
            return SetList {
                make_set_pair("annotatedElements", el.getAnnotatedElements())
            };
        }
    };
}

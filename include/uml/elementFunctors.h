#ifndef ELEMENT_FUNCTORS_H
#define ELEMENT_FUNCTORS_H

#include "sequence.h"
#include "relationship.h"
#include "umlManager.h"

namespace UML {
    class SetOwnerFunctor : public TemplateAbstractSequenceFunctor<Element,Element> {
        public:
            SetOwnerFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class RemoveOwnerFunctor : public TemplateAbstractSequenceFunctor<Element,Element> {
        public:
            RemoveOwnerFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Element& el) const override;
    };

    class AddRelationshipFunctor : public TemplateAbstractSequenceFunctor<Relationship,Element> {
        public:
            AddRelationshipFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Relationship& el) const override;
    };

    class RemoveRelationshipFunctor : public TemplateAbstractSequenceFunctor<Relationship,Element> {
        public:
            RemoveRelationshipFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Relationship& el) const override;
    };

    template <class T = Element> class ReadOnlySequenceFunctor : public TemplateAbstractSequenceFunctor<T,Element> {
        private:
            std::string m_name;
        public:
            ReadOnlySequenceFunctor<T>(Element* me, std::string sequenceName) : TemplateAbstractSequenceFunctor<T>(me) {
                m_name = sequenceName;
            };
            void operator()(T& el) const override {
                throw ReadOnlySequenceException(this->m_el->getID().string(), m_name);
            };
    };

    class AddDirectedRelationshipFunctor : public TemplateAbstractSequenceFunctor<DirectedRelationship,Element> {
        public:
            AddDirectedRelationshipFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(DirectedRelationship& el) const override;
    };

    class RemoveDirectedRelationshipFunctor : public TemplateAbstractSequenceFunctor<DirectedRelationship,Element> {
        public:
            RemoveDirectedRelationshipFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(DirectedRelationship& el) const override;
    };

    class AddOwnedCommentFunctor : public TemplateAbstractSequenceFunctor<Comment,Element> {
        public:
            AddOwnedCommentFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Comment& el) const override;
    };

    class RemoveOwnedCommentFunctor : public TemplateAbstractSequenceFunctor<Comment,Element> {
        public:
            RemoveOwnedCommentFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Comment& el) const override;
    };

    class AddAppliedStereotypeFunctor : public TemplateAbstractSequenceFunctor<InstanceSpecification,Element> {
        public:
            AddAppliedStereotypeFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(InstanceSpecification& el) const override;
    };

    class RemoveAppliedStereotypeFunctor : public TemplateAbstractSequenceFunctor<InstanceSpecification,Element> {
        public:
            RemoveAppliedStereotypeFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(InstanceSpecification& el) const override;
    };

    class CheckAppliedStereotypeFunctor : public TemplateAbstractSequenceFunctor<InstanceSpecification,Element> {
        public:
            CheckAppliedStereotypeFunctor(Element* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(InstanceSpecification& el) const override;
    };

    class InvalidAppliedStereotypeException : public std::exception {
        public:
            virtual const char* what() const throw() {
                return "tried to apply instance as stereotype that does not have a classifier set to a stereotype!";
            };
    };
}

#endif
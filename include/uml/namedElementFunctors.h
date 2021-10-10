#ifndef NAMED_ELEMENT_FUNCTORS_H
#define NAMED_ELEMENT_FUNCTORS_H

#include "sequence.h"
#include  "namedElement.h"

namespace UML {
    class AddMemberNamespaceFunctor : public TemplateAbstractSequenceFunctor<Namespace,NamedElement> {
        public:
            AddMemberNamespaceFunctor(NamedElement* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Namespace& el) const override;
    };
    class RemoveMemberNamespaceFunctor : public TemplateAbstractSequenceFunctor<Namespace,NamedElement> {
        public:
            RemoveMemberNamespaceFunctor(NamedElement* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Namespace& el) const override;
    };
    class AddClientDependencyFunctor : public TemplateAbstractSequenceFunctor<Dependency, NamedElement> {
        public:
            AddClientDependencyFunctor(NamedElement* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Dependency& el) const override;
            void operator()(ID id) const override;
    };
    class RemoveClientDependencyFunctor : public TemplateAbstractSequenceFunctor<Dependency, NamedElement> {
        public:
            RemoveClientDependencyFunctor(NamedElement* me) : TemplateAbstractSequenceFunctor(me) {};
            void operator()(Dependency& el) const override;
    };
}

#endif
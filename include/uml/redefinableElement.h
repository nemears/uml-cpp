#ifndef REDEFINABLE_ELEMENT_H
#define REDEFINABLE_ELEMENT_H

#include "sequence.h"

namespace UML {

    class Classifier;

    class RedefinableElement : virtual public NamedElement {
        friend class UmlManager;
        protected:
            Sequence<RedefinableElement> m_redefinedElement = Sequence<RedefinableElement>(this);
            Sequence<Classifier> m_redefinitionContext = Sequence<Classifier>(this);
            class AddRedefinedElementFunctor : public TemplateAbstractSequenceFunctor<RedefinableElement, RedefinableElement> {
                public:
                    AddRedefinedElementFunctor(RedefinableElement* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(RedefinableElement& el) const override;
            };
            class RemoveRedefinedElementFunctor : public TemplateAbstractSequenceFunctor<RedefinableElement, RedefinableElement> {
                public:
                    RemoveRedefinedElementFunctor(RedefinableElement* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(RedefinableElement& el) const override;
            };
            class AddRedefinitionContextFunctor : public TemplateAbstractSequenceFunctor<Classifier, RedefinableElement> {
                public:
                    AddRedefinitionContextFunctor(RedefinableElement* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Classifier& el) const override;
            };
            class RemoveRedefinitionContextFunctor : public TemplateAbstractSequenceFunctor<Classifier, RedefinableElement> {
                public:
                    RemoveRedefinitionContextFunctor(RedefinableElement* me) : TemplateAbstractSequenceFunctor(me) {};
                    void operator()(Classifier& el) const override;
            };
            void setManager(UmlManager* manager) override; // not polymorphic
            void reindexID(ID oldID, ID newID) override;
            void restoreReleased(ID id, Element* released) override;
            void referencingReleased(ID id) override;
            RedefinableElement();
        public:
            virtual ~RedefinableElement();
            RedefinableElement(const RedefinableElement& el);
            Sequence<RedefinableElement>& getRedefinedElements();
            Sequence<Classifier>& getRedefinitionContext();
            ElementType getElementType() const override;
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::REDEFINABLE_ELEMENT;
            };
    };
}

#endif
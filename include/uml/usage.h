#ifndef _UML_USAGE_H_
#define _UML_USAGE_H_

#include "dependency.h"

namespace UML {
    /**
     * A Usage is a Dependency in which one NamedElement requires another NamedElement (or set of NamedElements) for 
     * its full implementation or operation.
     **/
    class Usage : public Dependency {

        friend class UmlManager;

        protected:
            class RemoveClientFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    RemoveClientFunctor(Element* el) : SetFunctor(el) {};
            };
            class SetClientFunctor : public SetFunctor {
                private:
                    void operator()(Element& el) const override;
                public:
                    SetClientFunctor(Element* el) : SetFunctor(el) {};
            };
            void init();
            Usage();
        public:
            virtual ~Usage();
            bool isSubClassOf(ElementType eType) const override;
            static ElementType elementType() {
                return ElementType::USAGE;
            };
    };
}

#endif
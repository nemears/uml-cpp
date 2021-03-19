#ifndef NAMESPACEH
#define NAMESPACEH
#include <iostream>
#include "namedElement.h"
#include "sequence.h"
using namespace std;

namespace UML{
    /**
     * A Namespace is an Element in a model that contains a set of NamedElements that can be identified by name
     **/
    class Namespace : virtual public NamedElement {
        protected:
            Sequence<NamedElement>* m_members;
        public:
            Namespace();
            ~Namespace();
            Sequence<NamedElement>& getMembers();
            ElementType getElementType() override;
    };
}
#endif
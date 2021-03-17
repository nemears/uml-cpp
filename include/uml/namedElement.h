#ifndef NAMEDELEMENTH
#define NAMEDELEMENTH
#include <string>
#include "element.h"
using namespace std;

namespace UML{
    template <class T = Element> class Sequence;
    class NamedElement : virtual public Element {
        protected:
            string m_name;
            virtual void reindexName(string oldName, string newName);
        public:
            virtual string getName();
            virtual void setName(const string &name);
            ElementType getElementType() override;
            NamedElement(){};
    };
}

#endif
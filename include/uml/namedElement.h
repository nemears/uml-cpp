#ifndef NAMEDELEMENTH
#define NAMEDELEMENTH
#include <string>
#include "element.h"
using namespace std;

namespace UML{

    template <class T = Element> class Sequence;
    class Namespace;

    /**
     * A NamedElement is an Element in a model that may have a name
     **/
    class NamedElement : virtual public Element {
        protected:
            string m_name;
            Namespace* m_namespace;
            virtual void reindexName(string oldName, string newName);
            void reindexID(boost::uuids::uuid oldID, boost::uuids::uuid newID) override;
        public:
            NamedElement();
            virtual string getName();
            virtual void setName(const string &name);
            Namespace* getNamespace();
            void setNamespace(Namespace* nmspc);
            ElementType getElementType() override;
            bool isSubClassOf(ElementType eType) override;
    };
}

#endif
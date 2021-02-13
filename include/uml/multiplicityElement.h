#ifndef MULTIPLICITYELEMENT_H
#define MULTIPLICITYELEMENT_H

#include "element.h"

namespace UML {
    class MultiplicityElement : virtual public Element {
        public:
            int getLower();
            void setLower(const int low);
            int getUpper();
            void setUpper(const int up);
            class MultiplicityNotSpecifiedException: public exception {
                public:
                    virtual const char* what() const throw() {
                        return "tried to access multiplicity but it was not specified yet";
                    }
            } multiplicityNotSpecifiedException;
            bool multiplicitySpecified();
        protected:
            int lower;
            int upper;
        private:
            bool multiplicityIsSpecified = false;
            bool lowSpecified = false;
            bool upSpecified = false;
    };
}

#endif